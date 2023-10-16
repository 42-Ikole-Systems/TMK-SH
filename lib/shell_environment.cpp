
#include "shell/shell_environment.hpp"

#include <stdlib.h>
#include <sys/errno.h>
#include <cstring>
#include "shell/utility/types.hpp"
#include "shell/assert.hpp"

extern char *const *environ;

namespace shell {

shared_ptr<const char> ShellEnvironment::VariableMap::copyVariable(const string &variable) {
	auto copy = shared_ptr<const char>(new char[variable.length() + 1]);
	std::memcpy((char *)copy.get(), variable.data(), variable.length() + 1);
	return copy;
}

string_view ShellEnvironment::VariableMap::extractKey(const char *variable) {
	auto equal_sign = strchr(variable, '=');
	D_ASSERT(equal_sign);
	auto key_length = equal_sign - variable;
	string_view key(variable, key_length);
	return key;
}

void ShellEnvironment::VariableMap::add(const string &variable) {
	D_ASSERT(variable.find('=') != std::string::npos);
	auto str = copyVariable(variable);
	auto raw = (char*)str.get();

	// Find the key part
	auto key = extractKey(raw);

	auto equal_sign = key.data() + key.length();
	// Find the value part
	auto value_length = strlen(equal_sign + 1);
	string_view value(raw, value_length);

	auto it = owned_strings.insert(owned_strings.end(), str);
	auto entry = VariableEntry{
		it,
		value
	};
	// Finally add it to the environment map
	environ.emplace(std::make_pair(key, entry));
}

const char *ShellEnvironment::VariableMap::get(const string &name) {
	auto it = environ.find(name);
	if (it == environ.end()) {
		return nullptr;
	}
	return it->second.value_part.data();
}

void ShellEnvironment::VariableMap::update(const string &variable) {
	auto key = extractKey(variable.data());
	auto it = environ.find(key);
	if (it == environ.end()) {
		return;
	}
	remove(std::string(key));
	add(variable);
}

void ShellEnvironment::VariableMap::remove(const string &name) {
	auto it = environ.find(name);
	if (it == environ.end()) {
		return;
	}
	auto owned_string_entry = it->second.it;
	owned_strings.erase(owned_string_entry);
	environ.erase(it);
}

MaterializedEnvironment ShellEnvironment::VariableMap::materialize() {
	auto env = make_shared<EnvironmentVariables>();
	size_t env_size = environ.size();

	// Set up the map
	env->map = shared_ptr<char *const>(new char*[env_size + 1]);
	auto modifiable_map = (char**)env->map.get();

	// Deliminate it with null
	modifiable_map[env_size] = nullptr;

	// Populate the map and get a shared_ptr to the variables we hold
	size_t index = 0;
	for (auto &entry : owned_strings) {
		env->owned_strings.push_back(entry);
		modifiable_map[index++] = (char *)entry.get();
	}
	return env;
}

MaterializedEnvironment ShellEnvironment::getEnvironmentVariables() {
	if (!modified) {
		return cached_environment;
	}
	cached_environment = variable_map.materialize();
}

void ShellEnvironment::addEnvironmentVariable(const string &variable) {
	modified = true;
	variable_map.add(variable);
}

void ShellEnvironment::removeEnvironmentVariable(const string &name) {
	modified = true;
	variable_map.remove(name);
}

const char *ShellEnvironment::get(const string &name) {
	return variable_map.get(name);
}

} // namespace shell
