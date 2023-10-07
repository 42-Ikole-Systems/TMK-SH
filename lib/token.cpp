#include "lexer/token.hpp"
#include "logger.hpp"

namespace shell {

OperatorToken::OperatorToken() {
}

void Token::print() const {
	switch (type) {
		case TokenType::Word:
			word_token.print();
			break;
		case TokenType::Semicolon:
		case TokenType::And:
			operator_token.print(type);
			break;
		default:
			throw std::runtime_error("token not supported");
	}
}

void WordToken::print() const {
	LOG_DEBUG("Word(%)\n", value);
}

static char getChar(TokenType operator_type) {
	switch (operator_type) {
		case TokenType::Semicolon:
			return ';';
		case TokenType::And:
			return '&';
		default:
			throw std::runtime_error("invalid operator token ch");
	}
}

void OperatorToken::print(TokenType type) const {
	LOG_DEBUG("Operator(%)\n", getChar(type));
}

} // namespace shell
