#include "shell/grammar/rules/command_name.hpp"
#include "shell/grammar/rules/assignment_word.hpp"
#include "shell/assert.hpp"

namespace shell {

Rule CommandName::make() {
	return Rule {
	    CommandName::options,
	};
}

vector<Rule::Option> CommandName::options() {
	// TODO: implement
	return {Rule::Terminal {handler}};
}

optional<Ast::Node> CommandName::handler(TokenProvider &tokens) {
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
		// WORD is requested here
		return nullopt;
	}
	return Ast::Literal(std::move(token.value()));
}

} // namespace shell
