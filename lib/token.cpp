#include "lexer/token.hpp"
#include "logger.hpp"

namespace shell {

OperatorToken::OperatorToken(): type(OperatorToken::Type::Unknown) {}

OperatorToken::OperatorToken(char ch) {
    switch (ch) {
        case ';':
            type = OperatorToken::Type::Semicolon;
            break;
        case '&':
            type = OperatorToken::Type::Ampersand;
            break;
        default:
            throw std::runtime_error("invalid operator token ch");
    }
}

void Token::print() const {
    switch (type) {
        case Type::Word:
            word_token.print();
            break;
        case Type::Operator:
            operator_token.print();
            break;
    }
}

void WordToken::print() const {
    LOG_DEBUG("Word(%)\n", value);
}

static char getChar(OperatorToken::Type operator_type) {
    switch (operator_type) {
        case OperatorToken::Type::Semicolon:
            return ';';
        case OperatorToken::Type::Ampersand:
            return '&';
        case OperatorToken::Type::Unknown:
            throw std::runtime_error("invalid operator token ch");
    }
}

void OperatorToken::print() const {
    LOG_DEBUG("Operator(%)\n", getChar(type));
}

}
