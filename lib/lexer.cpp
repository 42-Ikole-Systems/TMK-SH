#include "lexer/lexer.hpp"
#include <stdexcept>
#include "util.hpp"
#include <stdio.h>

namespace shell {

Lexer::Lexer(Reader& reader) : reader(reader) {}

optional<Token> Lexer::getNextToken() {
    while (true) {
        char ch = reader.nextChar();
        if (ch == 0 || ch == '\n') {
            break;
        }
        printf("%c", ch);
        // token_parser->process(ch);
    }
    printf("\n");
    return nullopt;
}

vector<Token> Lexer::lexTokens(const string& line) {
    return {};
}

}; //namespace shell
