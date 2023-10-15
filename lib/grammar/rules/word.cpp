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
	auto token = tokens.peek();
	if (token->getType() != Token::Type::Token) {
		return nullopt;
	}
	tokens.consume();
	return Ast::Literal(std::move(token.value()));
}

} // namespace shell
