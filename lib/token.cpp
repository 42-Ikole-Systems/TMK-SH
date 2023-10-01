#include "lexer/token.hpp"
#include "print.hpp"

namespace shell {

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
    tprintf("Word(%)\n", value);
}

static char getChar(OperatorToken::Type operator_type) {
    switch (operator_type) {
        case OperatorToken::Type::Semicolon:
            return ';';
    }
}

void OperatorToken::print() const {
    tprintf("Operator(%)\n", getChar(type));
}

}
