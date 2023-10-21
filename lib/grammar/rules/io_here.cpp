#include "shell/grammar/rules/io_here.hpp"
#include "shell/grammar/rules/here_end.hpp"
#include "shell/grammar/rules/filename.hpp"
#include "shell/grammar/grammar_util.hpp"
#include "shell/assert.hpp"

namespace shell {

Rule IOHere::make() {
	return Rule {
	    IOHere::options,
	};
}

vector<Rule::Option> IOHere::options() {
	// TODO: implement
	return {Rule::NonTerminal(handler, {GrammarUtil::ConsumeIf<Token::Type::DoubleLess>::make(), HereEnd::make()}),
	        Rule::NonTerminal(handler, {GrammarUtil::ConsumeIf<Token::Type::DoubleLessDash>::make(), HereEnd::make()})};
}

optional<Ast::Node> IOHere::handler(vector<Ast::Node> &args) {
	return nullopt;
}

} // namespace shell
