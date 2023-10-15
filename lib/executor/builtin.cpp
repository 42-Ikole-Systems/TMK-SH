
#include "shell/executor/builtins.hpp"

namespace shell
{

	Builtin& Builtin::getInstance() {
		static Builtin builtin;
		return builtin;
	}

	Builtin::Builtin() {
		builtins.emplace("exit", [=](const Ast::Command& command) {
			exit(command.args);
			return ResultCode::Ok; // Does not actually get hit because it exits, its just here to keep the compiler happy :)
		});

		builtins.emplace("echo", [=](const Ast::Command& command) {
			return echo(command.args);
		});
	}

	optional<Builtin::builtinFunction> Builtin::getBuiltin(const string &program) {
		auto& that = getInstance();
		auto builtin = that.builtins.find(program);
		if (builtin != that.builtins.end()) {
			return builtin->second;
		}
		return nullopt;
	}

}
