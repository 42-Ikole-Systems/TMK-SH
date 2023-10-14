#include "shell/grammar/rules/simple_command.hpp"
#include "shell/grammar/rules/command_prefix.hpp"
#include "shell/grammar/rules/command_word.hpp"
#include "shell/grammar/rules/command_name.hpp"
#include "shell/grammar/rules/command_suffix.hpp"
#include "shell/grammar/grammar_util.hpp"

namespace shell {

Rule SimpleCommand::make() {
	return Rule {
	    SimpleCommand::options,
	};
}

vector<Rule::Option> SimpleCommand::options() {
	auto placeholder = [](vector<Ast::Node> &args) -> optional<Ast::Node> {
		return nullopt;
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
	        GrammarUtil::MakeOption<CommandName, CommandPrefix>(
	            // TODO implement
	            placeholder),
	        GrammarUtil::MakeOption<CommandName>(
	            // TODO implement
	            placeholder)};
}

} // namespace shell
