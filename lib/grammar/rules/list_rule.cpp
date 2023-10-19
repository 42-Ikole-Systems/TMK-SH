#include "shell/grammar/rules/list_rule.hpp"
#include "shell/grammar/rules/separator_op.hpp"
#include "shell/grammar/rules/and_or.hpp"
#include "shell/grammar/grammar_util.hpp"
#include "shell/assert.hpp"

namespace shell {

Rule ListRule::make() {
	return Rule {
	    ListRule::options,
	};
}

vector<Rule::Option> ListRule::options() {
	return {GrammarUtil::MakeOption<AndOr, SeparatorOp, ListRule>(ListRule::separatedList),
	        GrammarUtil::MakeOption<AndOr>(ListRule::singleCommand)};
}

optional<Ast::Node> ListRule::singleCommand(vector<Ast::Node> &nodes) {
	return std::move(nodes[0]);
}

optional<Ast::Node> ListRule::separatedList(vector<Ast::Node> &nodes) {
	D_ASSERT(nodes.size() == 3);

	auto &list = nodes[2];
	auto &command = nodes[0];

	auto separator = Ast::SeparatorOp();
	separator.left = make_unique<Ast::Node>(std::move(command));
	separator.right = make_unique<Ast::Node>(std::move(list));
	return separator;
}

} // namespace shell
