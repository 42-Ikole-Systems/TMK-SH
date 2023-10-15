#include "shell/grammar/rules/command_prefix.hpp"
#include "shell/grammar/rules/io_redirect.hpp"
#include "shell/grammar/rules/assignment_word.hpp"

namespace shell {

Rule CommandPrefix::make() {
	return Rule {
	    CommandPrefix::options,
	};
}

vector<Rule::Option> CommandPrefix::options() {
	auto placeholder = [](vector<Ast::Node> &args) -> optional<Ast::Node> {
		return std::move(args[0]);
	};
	return {Rule::NonTerminal(placeholder, {IORedirect::make(), CommandPrefix::make()}),
	        Rule::NonTerminal(placeholder, {AssignmentWord::make(), CommandPrefix::make()}),
	        Rule::NonTerminal(placeholder, {IORedirect::make()}),
	        Rule::NonTerminal(placeholder, {AssignmentWord::make()})};
}

} // namespace shell
