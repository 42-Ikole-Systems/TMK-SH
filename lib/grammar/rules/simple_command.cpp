#include "shell/grammar/rules/simple_command.hpp"
#include "shell/grammar/rules/command_prefix.hpp"
#include "shell/grammar/rules/command_word.hpp"
#include "shell/grammar/rules/command_name.hpp"
#include "shell/grammar/rules/command_suffix.hpp"
#include "shell/grammar/grammar_util.hpp"
#include "shell/print.hpp"

namespace shell {

Rule SimpleCommand::make() {
	return Rule {
	    SimpleCommand::options,
	};
}

vector<Rule::Option> SimpleCommand::options() {
	auto placeholder = [](vector<Ast::Node> &args) -> optional<Ast::Node> {
		throw std::runtime_error("Command with prefix not implemented yet");
	};
	return {GrammarUtil::MakeOption<CommandPrefix, CommandWord, CommandSuffix>(
	            // TODO implement
	            placeholder),
	        GrammarUtil::MakeOption<CommandPrefix, CommandWord>(
	            // TODO implement
	            placeholder),
	        GrammarUtil::MakeOption<CommandPrefix>(
	            // TODO implement
	            placeholder),
	        GrammarUtil::MakeOption<CommandName, CommandSuffix>(singleCommand),
	        GrammarUtil::MakeOption<CommandName>(singleCommand)};
}

optional<Ast::Node> SimpleCommand::singleCommand(vector<Ast::Node> &args) {
	vector<string> arguments;
	for (auto &arg : args) {
		auto &lit = arg.get<Ast::Literal>();
		auto &word_token = lit.token.get<WordToken>();
		tprintf("arg: %\n", word_token.value);
		arguments.push_back(word_token.value);
	}

	return Ast::Command(std::move(arguments));
}

} // namespace shell
