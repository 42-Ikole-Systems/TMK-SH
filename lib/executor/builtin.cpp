
#include "shell/executor/executor.hpp"

namespace shell
{

	bool Executor::isBuiltin(const string& program) {
		return builtins.count(program) > 0;
	}

	void Executor::generateBuiltinMap() {
		builtins.emplace("exit", [=](const Ast::Command& command) {
			Exit(command.args.size() > 1 ? static_cast<ResultCode>(std::stoi(command.args[1])) : ResultCode::Ok);
			return ResultCode::Ok; // Does not actually get hit because it exits, its just here to keep the compiler happy :)
		});
	}

	ResultCode Executor::executeBuiltin(const string& program, const Ast::Command& command)
	{
		auto& builtin = builtins.at(program);
		return builtin(command);
	}

}
