
#include "shell/environments/shell_environment.hpp"

#include <stdlib.h>
#include <sys/errno.h>
#include <cstring>
#include "shell/utility/types.hpp"
#include "shell/assert.hpp"

namespace shell {

ShellEnvironment::ShellEnvironment(char* const* envp) {
	if (!envp) {
		return;
	}
	for (size_t i = 0; envp[i]; i++) {
		variable_map.addUnsafe(envp[i]);
	}
}

shared_ptr<const char[]> ShellEnvironment::VariableMap::copyVariable(const string &variable) {
	auto copy = shared_ptr<const char[]>(new char[variable.length() + 1]);
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

std::pair<string_view, string_view> ShellEnvironment::VariableMap::getKeyValueParts(const char *str) {
	// Find the key part
	auto key = extractKey(str);

	auto equal_sign = key.data() + key.length();
	// Find the value part
	auto value_length = strlen(equal_sign + 1);
	string_view value(equal_sign + 1, value_length);
	return std::make_pair(key, value);
}

void ShellEnvironment::VariableMap::addUnsafe(const char *raw) {
	auto key_value = getKeyValueParts(raw);

	auto entry = VariableEntry {owned_strings.end(), key_value.second, true, true};
	// Finally add it to the environment map
	environment.emplace(std::make_pair(key_value.first, entry));
}

void ShellEnvironment::VariableMap::addInternal(const string &variable, bool exported) {
	D_ASSERT(variable.find('=') != std::string::npos);
	auto str = copyVariable(variable);
	auto raw = (char *)str.get();

	auto key_value = getKeyValueParts(raw);
	// Add it to the owned strings list
	auto it = owned_strings.insert(owned_strings.end(), str);
	auto entry = VariableEntry {it, key_value.second, exported, false};

	// Since our 'key' references memory we will free when the value gets deleted
	// every add also needs to be a delete, because the key needs to be updated as well..
	remove(key_value.first);

	// Finally add it to the environment map
	environment.emplace(std::make_pair(key_value.first, entry));
}

void ShellEnvironment::VariableMap::add(const string &variable) {
	addInternal(variable, false);
}

const char *ShellEnvironment::VariableMap::get(const string &name) const {
	auto it = environment.find(name);
	if (it == environment.end()) {
		return nullptr;
	}
	return it->second.value_part.data();
}

void ShellEnvironment::VariableMap::update(const string &variable) {
	auto key = extractKey(variable.data());
	auto it = environment.find(key);
	if (it == environment.end()) {
		return;
	}
	remove(std::string(key));
	add(variable);
}

void ShellEnvironment::VariableMap::markAsExported(const string &variable) {
	if (variable.find('=') == std::string::npos) {
		// Not an assignment, just marking an existing variable as exported
		auto it = environment.find(variable);
		if (it == environment.end()) {
			// Not an existing variable, just return
			return;
		}
		it->second.exported = true;
	} else {
		addInternal(variable, true);
	}
}

bool ShellEnvironment::VariableMap::isExported(const string &name) const {
	auto it = environment.find(name);
	if (it == environment.end()) {
		return false;
	}
	return it->second.exported;
}

void ShellEnvironment::VariableMap::remove(string_view name) {
	auto it = environment.find(name);
	if (it == environment.end()) {
		return;
	}
	if (!it->second.is_base) {
		// We own these strings, we are responsible for cleaning them up
		auto owned_string_entry = it->second.it;
		owned_strings.erase(owned_string_entry);
	}
	environment.erase(it);
}

void ShellEnvironment::VariableMap::remove(const string &name) {
	return remove(string_view(name.c_str(), name.length()));
}

MaterializedEnvironment ShellEnvironment::VariableMap::materialize() {
	auto env = make_shared<EnvironmentVariables>();
	size_t env_size = environment.size();

	// Set up the map
	env->map = shared_ptr<char *const[]>(new char *[env_size + 1]);
	auto modifiable_map = (char **)env->map.get();

	// Populate the map and get a shared_ptr to the variables we hold
	size_t index = 0;
	for (auto &[key, value] : environment) {
		if (!value.exported) {
			continue;
		}
		if (!value.is_base) {
			auto it = value.it;
			env->owned_strings.push_back(*it); // Share ownership with cache.
		}
		modifiable_map[index++] = (char *)key.data();
	}

	// Deliminate it with null
	modifiable_map[index] = nullptr;

	return env;
}

MaterializedEnvironment ShellEnvironment::materialize() {
	if (!modified) {
		return cached_environment;
	}
	cached_environment = variable_map.materialize();
	return cached_environment;
}

void ShellEnvironment::add(const string &variable) {
	modified = true;
	variable_map.add(variable);
}

void ShellEnvironment::remove(const string &name) {
	modified = true;
	variable_map.remove(name);
}

optional<string> ShellEnvironment::get(const string &name) const {
	auto res = variable_map.get(name);
	if (!res) {
		return nullopt;
	}
	return string(res);
}

void ShellEnvironment::exportVariable(const string &variable) {
	variable_map.markAsExported(variable);
}

bool ShellEnvironment::isExported(const string &name) const {
	return variable_map.isExported(name);
}

} // namespace shell
