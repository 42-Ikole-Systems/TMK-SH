#include "shell/grammar/rules/command_prefix.hpp"
#include "shell/grammar/rules/io_redirect.hpp"
#include "shell/grammar/rules/assignment_word.hpp"
#include "shell/assert.hpp"

namespace shell {

Rule CommandPrefix::make() {
	return Rule {
	    CommandPrefix::options,
	};
}

vector<Rule::Option> CommandPrefix::options() {
	auto handler = [](vector<Ast::Node> &args) -> optional<Ast::Node> {
		D_ASSERT(args.size() == 1);
		return Ast::List(std::move(args));
	};
	auto appender = [](vector<Ast::Node> &args) -> optional<Ast::Node> {
		D_ASSERT(args.size() == 2);
		auto &list_arg = args[1].get<Ast::List>();

		list_arg.append(std::move(args[0]));
		return std::move(list_arg);
	};
	return {Rule::NonTerminal(appender, {IORedirect::make(), CommandPrefix::make()}),
	        Rule::NonTerminal(appender, {AssignmentWord::make(), CommandPrefix::make()}),
	        Rule::NonTerminal(handler, {IORedirect::make()}), Rule::NonTerminal(handler, {AssignmentWord::make()})};
}

} // namespace shell
