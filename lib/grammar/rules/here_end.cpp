#include "shell/grammar/rules/here_end.hpp"
#include "shell/grammar/grammar_util.hpp"
#include "shell/assert.hpp"

namespace shell {

Rule HereEnd::make() {
	return Rule {
	    HereEnd::options,
	};
}

vector<Rule::Option> HereEnd::options() {
	return {Rule::Terminal {handler}};
}

optional<Ast::Node> HereEnd::handler(TokenProvider &tokens) {
	// TODO: implement
	// Rule 3
	return nullopt;
}

} // namespace shell
