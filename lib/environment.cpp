
#include "shell/environment.hpp"

#include <stdlib.h>
#include <sys/errno.h>
#include <cstring>

extern char *const *environ;

namespace shell {

char *const *Environment::getEnvironmentVariables() {
	return environ;
}

void Environment::addEnvironmentVariable(const string &variable) {
	char *raw = new char[variable.length() + 1]; // Dont delete because it becomes part of env.
	std::memcpy(raw, variable.data(), variable.length() + 1);
	if (putenv(raw) == -1) {
		delete[] raw;
		throw std::runtime_error(strerror(errno));
	}
}

void Environment::removeEnvironmentVariable(const string &name) {
	if (unsetenv(name.c_str()) == -1) {
		throw std::runtime_error(strerror(errno));
	}
}

const char *Environment::get(const string &name) {
	return getenv(name.c_str());
}

} // namespace shell
