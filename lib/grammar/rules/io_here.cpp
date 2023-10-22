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
	// TODO:
	// If the redirection operator is "<<" or "<<-",
	// the word that follows the redirection operator shall be subjected to quote removal;

	// it is unspecified whether any of the other expansions occur
	return nullopt;
}

} // namespace shell
