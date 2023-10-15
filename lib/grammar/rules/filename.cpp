#include "shell/grammar/rules/filename.hpp"

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
	auto token = tokens.peek();
	if (token->getType() != Token::Type::Token) {
		return nullopt;
	}
	// Rule 2
	// shall be subjected to tilde expansion, parameter expansion, command substitution, arithmetic expansion, and quote
	// removal
	// TODO: implement the expansions
	return Ast::Literal(std::move(tokens.consume().value()));
}

} // namespace shell
