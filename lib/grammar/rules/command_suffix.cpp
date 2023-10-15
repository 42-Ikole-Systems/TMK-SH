#include "shell/grammar/rules/command_suffix.hpp"
#include "shell/grammar/rules/io_redirect.hpp"
#include "shell/grammar/rules/word.hpp"
#include "shell/assert.hpp"

namespace shell {

Rule CommandSuffix::make() {
	return Rule {
	    CommandSuffix::options,
	};
}

vector<Rule::Option> CommandSuffix::options() {
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

	return {Rule::NonTerminal(appender, {IORedirect::make(), CommandSuffix::make()}),
	        Rule::NonTerminal(appender, {Word::make(), CommandSuffix::make()}),
	        Rule::NonTerminal(handler, {IORedirect::make()}), Rule::NonTerminal(handler, {Word::make()})};
}

} // namespace shell
