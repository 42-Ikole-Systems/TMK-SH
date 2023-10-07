#include "grammar/rules/list_rule.hpp"
#include "grammar/rules/separator_op.hpp"
#include "grammar/rules/and_or.hpp"
#include "grammar/rule_producer.hpp"

namespace shell {

Rule ListRule::make() {
	return Rule {
	    ListRule::options,
	};
}

vector<Rule::Option> ListRule::options() {
	return {RuleProducer::MakeOption<AndOr, SeparatorOp, ListRule>(ListRule::separatedList),
	        RuleProducer::MakeOption<AndOr>(ListRule::singleCommand)};
}

optional<Ast::Node> ListRule::singleCommand(const vector<Ast::Node> &nodes) {
	return nullopt;
}

optional<Ast::Node> ListRule::separatedList(const vector<Ast::Node> &nodes) {
	return nullopt;
}

} // namespace shell
