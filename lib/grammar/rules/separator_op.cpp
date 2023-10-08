#include "shell/grammar/rules/separator_op.hpp"
#include "shell/grammar/grammar_util.hpp"

namespace shell {

Rule SeparatorOp::make() {
	return Rule {
	    SeparatorOp::options,
	};
}

vector<Rule::Option> SeparatorOp::options() {
	return {Rule::Terminal {GrammarUtil::ConsumeIf<Token::Type::Semicolon>},
	        Rule::Terminal {GrammarUtil::ConsumeIf<Token::Type::And>}};
}

} // namespace shell
