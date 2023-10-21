#pragma once

#include "shell/ast.hpp"
#include "shell/environment.hpp"

namespace shell {

// https://www.gnu.org/software/bash/manual/bash.html#Exit-Status
enum class ResultCode : uint8_t {
	Ok = 0,
	GeneralError = 1,
	IncorrectUsage = 2, // built-in errors
	CommandNotExecutable = 126,
	CommandNotFound = 127,
	FatalSignal = 128 // exit status = 128 + (signal value)
};

class Executor {
private:
public:
	Executor(Environment &env);

	ResultCode execute(Ast &ast);
	ResultCode execute(Ast::Node &node);
	ResultCode execute(Ast::Command &node);
	ResultCode execute(Ast::SeparatorOp &node);

private:
	[[noreturn]] void Exit(ResultCode code);
	optional<string> resolvePath(const string &program);

private:
	Environment &env;
};

} // namespace shell
