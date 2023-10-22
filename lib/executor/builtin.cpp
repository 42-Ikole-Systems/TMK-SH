
#include "shell/executor/builtins.hpp"
#include "shell/assert.hpp"

namespace shell {

static vector<string> getArgumentsAsString(const Ast::Command &command) {
	vector<string> args;
	for (auto &entry : command.arguments.entries) {
		D_ASSERT(entry.getType() == Ast::Node::Type::Literal);
		args.push_back(entry.get<Ast::Literal>().token.get<WordToken>().value.c_str());
	}
	return args;
}

std::map<string, Builtin::builtinFunction> Builtin::createBuiltinLookup() {
	std::map<string, builtinFunction> builtins;
	builtins.emplace("exit", [](const Ast::Command &command, Environment &) {
		Builtin::exit(getArgumentsAsString(command));
		return ResultCode::Ok; // Does not actually get hit because it exits, its just here to keep the compiler happy
		                       // :)
	});

	builtins.emplace("echo", [](const Ast::Command &command, Environment &) {
		return Builtin::echo(getArgumentsAsString(command));
	});

	builtins.emplace("export", [](const Ast::Command &command, Environment &environment) {
		auto args = getArgumentsAsString(command);
		return Builtin::export_variable(args, environment);
	});

	builtins.emplace("set", [](const Ast::Command &command, Environment &environment) {
		auto args = getArgumentsAsString(command);
		return Builtin::set_variable(args, environment);
	});

	builtins.emplace("unset", [](const Ast::Command &command, Environment &environment) {
		auto args = getArgumentsAsString(command);
		return Builtin::unset_variable(args, environment);
	});

	builtins.emplace("getenv", [](const Ast::Command &command, Environment &environment) {
		auto args = getArgumentsAsString(command);
		return Builtin::get_variable(args, environment);
	});
	return builtins;
}

optional<Builtin::builtinFunction> Builtin::getBuiltin(const string &program) {
	static auto builtins = createBuiltinLookup();
	auto builtin = builtins.find(program);
	if (builtin != builtins.end()) {
		return builtin->second;
	}
	return nullopt;
}

} // namespace shell
