
#include "shell/executor/builtins.hpp"

#include <iostream>

namespace shell {
ResultCode Builtin::get_variable(const vector<string> &args, Environment &environment) {
	if (args.size() != 1) {
		throw NotImplementedException("Getenv needs an argument");
	}
	auto &argument = args[0];
	auto res = environment.get(argument);
	auto is_exported = environment.isExported(argument);
	std::cout << res << " " << (is_exported ? "exported(true)" : "exported(false)") << std::endl;
	return ResultCode::Ok;
}
} // namespace shell
