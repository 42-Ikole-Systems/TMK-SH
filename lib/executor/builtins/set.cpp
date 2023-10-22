
#include "shell/executor/builtins.hpp"

#include <iostream>

namespace shell {
ResultCode Builtin::set_variable(const vector<string> &args, Environment &environment) {
	if (args.size() != 1) {
		throw NotImplementedException("Setting a local variable only supports a single argument for now");
	}
	auto &argument = args[0];
	auto equal_sign_pos = argument.find('=');
	if (equal_sign_pos == std::string::npos) {
		// FIXME: support 'set var'
		// this should set the value to NULL for 'var'
		return ResultCode::IncorrectUsage;
	}
	environment.add(argument);
	return ResultCode::Ok;
}
} // namespace shell
