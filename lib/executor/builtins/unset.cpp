
#include "shell/executor/builtins.hpp"

#include <iostream>

namespace shell {
ResultCode Builtin::unset_variable(const vector<string> &args, Environment &environment) {
	if (args.size() != 1) {
		throw NotImplementedException("Export only supports a single argument for now");
	}
	auto &argument = args[0];
	// FIXME: should probably check for '=' and error if it's present??
	environment.remove(argument);
	return ResultCode::Ok;
}
} // namespace shell
