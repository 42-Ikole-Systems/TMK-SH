
#include "shell/global_environment.hpp"

#include <stdlib.h>
#include <sys/errno.h>
#include <cstring>
#include "shell/assert.hpp"

extern char *const *environ;

namespace shell {

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
	auto res = getenv(name.c_str());
	if (!res) {
		return string();
	}
	return string(res);
}

void GlobalEnvironment::add(const string &variable) {
	auto keyval = getKeyValueParts(variable.c_str());
	auto key = string(keyval.first);
	auto value = string(keyval.second);
	setenv(key.c_str(), value.c_str(), true);
}

void GlobalEnvironment::remove(const string &name) {
	if (unsetenv(name.c_str()) == -1) {
		throw std::runtime_error(strerror(errno));
	}
}

void GlobalEnvironment::setExportTag(const string &name) {
	return;
}

char *const *GlobalEnvironment::materialize() const {
	return environ;
}

} // namespace shell
