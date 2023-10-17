#include "shell/grammar/rules/word.hpp"
#include "shell/interfaces/provider.hpp"
#include "shell/assert.hpp"

namespace shell {

Rule Word::make() {
	return Rule {
	    Word::options,
	};
}

vector<Rule::Option> Word::options() {
	return {Rule::Terminal {handler}};
}

optional<Ast::Node> Word::handler(TokenProvider &tokens) {
	auto token = tokens.consumeIf([](Token &token) { return token.getType() == Token::Type::Token; });
	if (!token.has_value()) {
		return nullopt;
	}
	return Ast::Literal(Token(Token::Type::Word, WordToken {token->get<WordToken>().value}));
}

} // namespace shell
