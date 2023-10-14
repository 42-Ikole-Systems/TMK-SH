#include "shell/grammar/rules/command_suffix.hpp"
#include "shell/grammar/rules/io_redirect.hpp"
#include "shell/grammar/rules/word.hpp"

namespace shell {

Rule CommandSuffix::make() {
	return Rule {
	    CommandSuffix::options,
	};
}

vector<Rule::Option> CommandSuffix::options() {
	// TODO: implement
	auto placeholder = [](vector<Ast::Node> &args) -> optional<Ast::Node> {
		return std::move(args[0]);
	};
	// FIXME: how do we implement lists of command prefixes?
	return {Rule::NonTerminal(placeholder, {IORedirect::make()}), Rule::NonTerminal(placeholder, {Word::make()})};
}

} // namespace shell
