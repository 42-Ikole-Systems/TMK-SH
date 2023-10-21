
#include "shell/executor/builtins.hpp"
#include "shell/assert.hpp"

namespace shell
{

	Builtin& Builtin::getInstance() {
		static Builtin builtin;
		return builtin;
	}

	static vector<string> getArgumentsAsString(const Ast::Command& command)
	{
		vector<string> args;
		for (auto &entry : command.arguments.entries) {
			D_ASSERT(entry.getType() == Ast::Node::Type::Literal);
			args.push_back(entry.get<Ast::Literal>().token.get<WordToken>().value.c_str());
		}
		return args;
	}

	Builtin::Builtin() {
		builtins.emplace("exit", [=](const Ast::Command& command, Environment&) {
			Builtin::exit(getArgumentsAsString(command));
			return ResultCode::Ok; // Does not actually get hit because it exits, its just here to keep the compiler happy :)
		});

		builtins.emplace("echo", [=](const Ast::Command& command, Environment&) {
			return Builtin::echo(getArgumentsAsString(command));
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
