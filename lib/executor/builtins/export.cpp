
#include "shell/executor/builtins.hpp"

#include <iostream>

namespace shell {
ResultCode Builtin::export_variable(const vector<string> &args, Environment &environment) {
	if (args.size() != 1) {
		throw NotImplementedException("Export only supports a single argument for now");
	}
	auto &argument = args[0];
	environment.exportVariable(argument);
	return ResultCode::Ok;
}
} // namespace shell
