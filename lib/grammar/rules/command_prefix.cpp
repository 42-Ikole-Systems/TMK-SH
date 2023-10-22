#include "shell/grammar/rules/command_prefix.hpp"
#include "shell/grammar/rules/io_redirect.hpp"
#include "shell/grammar/rules/assignment_word.hpp"
#include "shell/grammar/list.hpp"
#include "shell/assert.hpp"

namespace shell {

Rule CommandPrefix::make() {
	return Rule {
	    CommandPrefix::options,
	};
}

vector<Rule::Option> CommandPrefix::options() {

	auto redirection = [](Ast::Node node) -> Ast::Node {
		return node;
	};
	auto assignment = [](Ast::Node node) -> Ast::Node {
		// TODO:
		// Each variable assignment shall be expanded:
		// - tilde expansion
		// - parameter expansion
		// - command substitution
		// - arithmetic expansion
		// - and quote removal
		return node;
	};

	return {
	    Rule::NonTerminal(ListConstructor::ListAppender(redirection), {IORedirect::make(), CommandPrefix::make()}),
	    Rule::NonTerminal(ListConstructor::ListAppender(assignment), {AssignmentWord::make(), CommandPrefix::make()}),
	    Rule::NonTerminal(ListConstructor::ListCreator(redirection), {IORedirect::make()}),
	    Rule::NonTerminal(ListConstructor::ListCreator(assignment), {AssignmentWord::make()})};
}

} // namespace shell
