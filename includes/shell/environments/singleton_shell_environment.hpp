#pragma once

#include "shell/utility/types.hpp"
#include "shell/environments/shell_environment.hpp"

namespace shell {

class SingletonShellEnvironment : public ShellEnvironment {

	SingletonShellEnvironment();

public:
	static SingletonShellEnvironment& getInstance();
};

}