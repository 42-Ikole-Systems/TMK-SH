#include "lexer/token.hpp"
#include "logger.hpp"

namespace shell {

Token::Token(Token::Type type, Variant &&variant) : type(type), variant(std::move(variant)) {
}

static char getOperatorChar(Token::Type operator_type) {
	switch (operator_type) {
		case Token::Type::Semicolon:
			return ';';
		case Token::Type::And:
			return '&';
		default:
			throw std::runtime_error("invalid operator token ch");
	}
}

void Token::print() const {
	switch (type) {
		case Token::Type::Word:
			get<WordToken>().print();
			break;
		case Token::Type::Semicolon:
		case Token::Type::And:
			LOG_DEBUG("Operator(%)\n", getOperatorChar(type));
			break;
		case Token::Type::Newline:
			LOG_DEBUG("Newline(\\n)\n");
			break;
		default:
			throw std::runtime_error("token not supported");
	}
}

Token::Type Token::getType() const {
	return type;
}

void WordToken::print() const {
	LOG_DEBUG("Word(%)\n", value);
}

} // namespace shell
