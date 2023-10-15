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
	if (token->getType() != Token::Type::Word) {
		return nullopt;
	}
	return Ast::Literal(std::move(token.value()));
}

} // namespace shell
