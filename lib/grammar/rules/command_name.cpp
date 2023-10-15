#include "shell/grammar/rules/command_name.hpp"
#include "shell/grammar/rules/assignment_word.hpp"
#include "shell/assert.hpp"
#include "shell/grammar/grammar_util.hpp"

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
	auto token = tokens.consumeIf([](Token &token) { return token.getType() == Token::Type::Token; });
	if (!token.has_value()) {
		return nullopt;
	}
	// Rule 7a
	auto &word_token = token->get<WordToken>();
	auto &value = word_token.value;
	if (value.find('=') == std::string::npos) {
		// Rule 1
		auto reserved_word = Token::exactReservedWordType(value);
		if (!reserved_word.has_value()) {
			return Ast::Literal(Token(Token::Type::Word, WordToken {word_token.value}));
		}
		// WORD is requested here
		return nullopt;
	}
	// Rule 7b
	D_ASSERT(!value.empty());
	if (value[0] == '=') {
		return Ast::Literal(Token(Token::Type::Word, WordToken {word_token.value}));
	}
	auto equals_pos = value.find('=');
	D_ASSERT(equals_pos != std::string::npos);
	auto maybe_name = value.substr(0, equals_pos);
	if (AssignmentWord::isValidName(maybe_name)) {
		// WORD is requested here
		return nullopt;
	}
	return Ast::Literal(Token(Token::Type::Word, WordToken {word_token.value}));
}

} // namespace shell
