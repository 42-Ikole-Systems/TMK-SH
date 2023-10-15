#include "shell/grammar/rules/filename.hpp"
#include "shell/grammar/grammar_util.hpp"

namespace shell {

Rule Filename::make() {
	return Rule {
	    Filename::options,
	};
}

vector<Rule::Option> Filename::options() {
	return {Rule::Terminal {handler}};
}

optional<Ast::Node> Filename::handler(TokenProvider &tokens) {
	auto token = tokens.consumeIf([](Token &token) { return token.getType() == Token::Type::Token; });
	if (!token.has_value()) {
		return nullopt;
	}
	// Rule 2
	// shall be subjected to tilde expansion, parameter expansion, command substitution, arithmetic expansion, and quote
	// removal
	// TODO: implement the expansions
	return Ast::Literal(Token(Token::Type::Word, WordToken {token->get<WordToken>().value}));
}

} // namespace shell
