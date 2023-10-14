#include "shell/grammar/rules/word.hpp"
#include "shell/grammar/rules/assignment_word.hpp"
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
	// Rule 7a
	auto &word_token = token->get<WordToken>();
	tokens.consume();
	auto &value = word_token.value;
	if (value.find('=') == std::string::npos) {
		// Rule 1
		auto reserved_word = Token::exactReservedWordType(value);
		if (!reserved_word.has_value()) {
			return Ast::Literal(std::move(token.value()));
		}
		// WORD is requested here
		return nullopt;
	}
	// Rule 7b
	D_ASSERT(!value.empty());
	if (value[0] == '=') {
		return Ast::Literal(std::move(token.value()));
	}
	auto equals_pos = value.find('=');
	D_ASSERT(equals_pos != std::string::npos);
	auto maybe_name = value.substr(0, equals_pos);
	if (AssignmentWord::isValidName(maybe_name)) {
		return nullopt;
	}
	return Ast::Literal(std::move(token.value()));
}

} // namespace shell
