#pragma once

#include "shell/utility/types.hpp"
#include "shell/environments/shell_environment.hpp"

namespace shell {

class SingletonShellEnvironment : public ShellEnvironment {

	SingletonShellEnvironment();
	SingletonShellEnvironment(const SingletonShellEnvironment&) = delete;
	SingletonShellEnvironment(SingletonShellEnvironment&&) = delete;
	SingletonShellEnvironment& operator = (const SingletonShellEnvironment&) = delete;
	~SingletonShellEnvironment() = default;

public:
	static SingletonShellEnvironment& getInstance();
};

}