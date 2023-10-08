#include "shell/rules/and_or.hpp"

namespace shell {

GrammarRule AndOr::make() {
	return GrammarRule {
	    AndOr::options,
	};
}

vector<GrammarRule::Option> AndOr::options() {
	return {GrammarRule::MakeOption<AndOr>(AndOr::handler)};
}

optional<Ast::Node> AndOr::handler(const vector<Ast::Node> &nodes) {
	return nullopt;
}

} // namespace shell
