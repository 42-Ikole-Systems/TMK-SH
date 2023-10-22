#include "shell/environments/singleton_shell_environment.hpp"

extern char *const *environ;

namespace shell
{
	SingletonShellEnvironment::SingletonShellEnvironment() : ShellEnvironment(environ) {
	}

	SingletonShellEnvironment& SingletonShellEnvironment::getInstance() {
		static SingletonShellEnvironment environment;
		return environment;
	}
}
