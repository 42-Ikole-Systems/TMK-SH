#include "shell/rules/separator_op.hpp"

namespace shell {

GrammarRule SeparatorOp::make() {
	return GrammarRule {
	    SeparatorOp::options,
	};
}

vector<GrammarRule::Option> SeparatorOp::options() {
	return {GrammarRule::MakeOption<SeparatorOp>(SeparatorOp::handler)};
}

optional<Ast::Node> SeparatorOp::handler(const vector<Ast::Node> &nodes) {
	return nullopt;
}

} // namespace shell
