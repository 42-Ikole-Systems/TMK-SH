#include "shell/grammar/rules/command_prefix.hpp"
#include "shell/grammar/rules/io_redirect.hpp"
#include "shell/grammar/rules/assignment_word.hpp"

namespace shell {

Rule CommandPrefix::make() {
	return Rule {
	    CommandPrefix::options,
	};
}

// cmd_prefix       :            io_redirect
//                 | cmd_prefix io_redirect
//                 |            ASSIGNMENT_WORD
//                 | cmd_prefix ASSIGNMENT_WORD
vector<Rule::Option> CommandPrefix::options() {
	// TODO: implement
	auto placeholder = [](vector<Ast::Node> &args) -> optional<Ast::Node> {
		return std::move(args[0]);
	};
	// FIXME: how do we implement lists of command prefixes?
	return {Rule::NonTerminal(placeholder, {IORedirect::make()}),
	        Rule::NonTerminal(placeholder, {AssignmentWord::make()})};
}

} // namespace shell
