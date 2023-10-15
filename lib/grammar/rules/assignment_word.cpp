#include "shell/grammar/rules/assignment_word.hpp"
#include "shell/interfaces/provider.hpp"
#include "shell/assert.hpp"
#include "shell/grammar/grammar_util.hpp"

namespace shell {

Rule AssignmentWord::make() {
	return Rule {
	    AssignmentWord::options,
	};
}

vector<Rule::Option> AssignmentWord::options() {
	return {Rule::Terminal {handler}};
}

bool AssignmentWord::isValidName(const string &input) {
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

optional<Ast::Node> AssignmentWord::handler(TokenProvider &tokens) {
	auto token = tokens.consumeIf([](Token &token) { return token.getType() == Token::Type::Token; });
	if (!token.has_value()) {
		return nullopt;
	}
	// Rule 7a
	auto &word_token = token->get<WordToken>();
	auto &value = word_token.value;
	if (value.find('=') == std::string::npos) {
		// Do not apply Rule 7a
		return nullopt;
	}
	// Rule 7b
	D_ASSERT(!value.empty());
	if (value[0] == '=') {
		// Do not fall back to anything that isn't AssignmentWord
		return nullopt;
	}
	auto equals_pos = value.find('=');
	D_ASSERT(equals_pos != std::string::npos);
	auto maybe_name = value.substr(0, equals_pos);
	if (isValidName(maybe_name)) {
		return Ast::Literal(Token(Token::Type::AssignmentWord, WordToken {value}));
	}
	// Do not fall back to anything that isn't AssignmentWord
	// FIXME: it is unspecified whether this should return AssignmentWord or Word
	// maybe we want to return AssignmentWord as that is being requested here
	return nullopt;
}

} // namespace shell
