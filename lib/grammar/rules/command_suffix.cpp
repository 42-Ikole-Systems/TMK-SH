#include "shell/grammar/rules/command_suffix.hpp"
#include "shell/grammar/rules/io_redirect.hpp"
#include "shell/grammar/rules/word.hpp"
#include "shell/grammar/list.hpp"
#include "shell/assert.hpp"

namespace shell {

Rule CommandSuffix::make() {
	return Rule {
	    CommandSuffix::options,
	};
}

vector<Rule::Option> CommandSuffix::options() {

	auto redirection = [](Ast::Node node) -> Ast::Node {
		return node;
	};
	auto word = [](Ast::Node node) -> Ast::Node {
		return node;
	};

	return {Rule::NonTerminal(ListConstructor::ListAppender(redirection), {IORedirect::make(), CommandSuffix::make()}),
	        Rule::NonTerminal(ListConstructor::ListAppender(word), {Word::make(), CommandSuffix::make()}),
	        Rule::NonTerminal(ListConstructor::ListCreator(redirection), {IORedirect::make()}),
	        Rule::NonTerminal(ListConstructor::ListCreator(word), {Word::make()})};
}

} // namespace shell
