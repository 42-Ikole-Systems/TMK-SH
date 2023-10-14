#include "shell/grammar/rules/command_name.hpp"
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

static bool isValidName(const string &input) {
	if (input.empty()) {
		return false;
	}
	auto first_char = input[0];
	if (isdigit(first_char)) {
		return false;
	}
	for (auto &ch : input) {
		auto is_underscore = ch == '_';
		auto is_digit = isdigit(ch);
		auto is_alpha = isalpha(ch);
		if (!is_underscore && is_digit && is_alpha) {
			return false;
		}
	}
	return true;
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
		return Ast::Literal(Token(reserved_word.value(), ReservedWordToken()));
	}
	// Rule 7b
	D_ASSERT(!value.empty());
	if (value[0] == '=') {
		return Ast::Literal(std::move(token.value()));
	}
	auto equals_pos = value.find('=');
	D_ASSERT(equals_pos != std::string::npos);
	auto maybe_name = value.substr(0, equals_pos);
	if (isValidName(maybe_name)) {
		return Ast::Literal(Token(Token::Type::AssignmentWord, WordToken {value}));
	}
	return Ast::Literal(std::move(token.value()));
}

} // namespace shell
