
#include "shell/global_environment.hpp"

#include <stdlib.h>
#include <sys/errno.h>
#include <cstring>
#include "shell/assert.hpp"

extern char *const *environ;

namespace shell {

GlobalEnvironment::GlobalEnvironment() {
	for (size_t i = 0; environ[i]; i++) {
		auto key = extractKey(environ[i]);
		// All initial variables are exported
		variables[string(key)] = VariableData(true);
	}
}

string_view GlobalEnvironment::extractKey(const char *variable) {
	auto equal_sign = strchr(variable, '=');
	D_ASSERT(equal_sign);
	auto key_length = equal_sign - variable;
	string_view key(variable, key_length);
	return key;
}

std::pair<string_view, string_view> GlobalEnvironment::getKeyValueParts(const char *str) {
	// Find the key part
	auto key = extractKey(str);
	auto equal_sign = key.data() + key.length();
	// Find the value part
	auto value_length = strlen(equal_sign + 1);
	string_view value(equal_sign + 1, value_length);
	return std::make_pair(key, value);
}

string GlobalEnvironment::get(const string &name) const {
	// FIXME: this only returns the value, not the key=value
	auto res = getenv(name.c_str());
	if (!res) {
		return string();
	}
	return string(res);
}

void GlobalEnvironment::addInternal(const string &variable, bool exported) {
	auto keyval = getKeyValueParts(variable.c_str());
	auto key = string(keyval.first);
	auto value = string(keyval.second);
	setenv(key.c_str(), value.c_str(), true);
	variables[key] = VariableData(exported);
}

void GlobalEnvironment::add(const string &variable) {
	addInternal(variable, false);
}

void GlobalEnvironment::remove(const string &name) {
	if (unsetenv(name.c_str()) == -1) {
		throw std::runtime_error(strerror(errno));
	}
	variables.erase(name);
}

void GlobalEnvironment::exportVariable(const string &variable) {
	if (variable.find('=') == std::string::npos) {
		// Not an assignment, just marking an existing variable as exported
		auto it = variables.find(variable);
		if (it == variables.end()) {
			// Not an existing variable, just return
			return;
		}
		it->second.exported = true;
	} else {
		addInternal(variable, true);
	}
}

bool GlobalEnvironment::isExported(const string &name) const {
	D_ASSERT(name.find('=') == std::string::npos);
	auto it = variables.find(name);
	if (it == variables.end()) {
		return false;
	}
	return it->second.exported == true;
}

MaterializedEnvironment GlobalEnvironment::materialize() {
	// For the allocation, lets just assume everything is exported, doesn't matter if we overshoot a little
	auto environment = make_shared<EnvironmentVariables>();
	environment->map = shared_ptr<char *const[]>(new char *[variables.size() + 1]);
	auto envs = (char **)environment->map.get();
	size_t idx = 0;
	for (auto &var : variables) {
		if (!var.second.exported) {
			continue;
		}
		D_ASSERT(getenv(var.first.c_str()));
		envs[idx++] = getenv(var.first.c_str());
	}
	envs[idx] = nullptr;
	return environment;
}

} // namespace shell
