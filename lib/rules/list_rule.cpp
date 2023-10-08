#include "shell/rules/list_rule.hpp"
#include "shell/rules/separator_op.hpp"
#include "shell/rules/and_or.hpp"

namespace shell {

GrammarRule ListRule::make() {
	return GrammarRule {
	    ListRule::options,
	};
}

vector<GrammarRule::Option> ListRule::options() {
	return {GrammarRule::MakeOption<AndOr, SeparatorOp, ListRule>(ListRule::separatedList),
	        GrammarRule::MakeOption<AndOr>(ListRule::singleCommand)};
}

optional<Ast::Node> ListRule::singleCommand(const vector<Ast::Node> &nodes) {
	return nullopt;
}

optional<Ast::Node> ListRule::separatedList(const vector<Ast::Node> &nodes) {
	return nullopt;
}

} // namespace shell
