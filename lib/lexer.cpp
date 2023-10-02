#include "lexer/lexer.hpp"
#include <stdexcept>
#include "util.hpp"
#include <stdio.h>
#include <cassert>
#include "logger.hpp"

namespace shell {

Lexer::Lexer(Provider<char>& chars, State state) : chars(chars), state(state) {}
Lexer::Lexer(Provider<char>& chars) : Lexer(chars, State::Empty) {}

optional<Token> Lexer::peek() {
    if (!token.has_value()) {
        token = nextToken();
    }
    return token;
}

optional<Token> Lexer::consume() {
    // exchange
    optional<Token> next = nullopt;
    token.swap(next);
    return next;
}

optional<Token> Lexer::nextToken() {
    while (state != State::Done) {
        auto handler = getStateHandler();
        state = handler(*this);
        if (!tokens.empty()) {
            assert(tokens.size() == 1); // disallow state to add multiple tokens?
            // pop next token from queue and return it
            Token token = tokens.front();
            tokens.pop();
            return token;
        }
    }
    return nullopt;
}

void Lexer::delimit(Token&& token) {
    tokens.emplace(token);
}

std::function<Lexer::State(Lexer&)> Lexer::getStateHandler() {
    static const std::function<State(Lexer&)> handlers[] = {
        [Empty] = &Lexer::emptyState,
        [Word] = &Lexer::wordState,
        [Operator] = &Lexer::operatorState,
    };
    return handlers[state];
}

Lexer::State Lexer::emptyState() {
    char ch = chars.peek();
    if (ch == EOF) {
        return State::Done;
    } else if (isSpace(ch)) {
        chars.consume();
        return State::Empty;
    } else if (isMetaCharacter(ch)) {
        return State::Operator;
    } else {
        return State::Word;
    }
}

Lexer::State Lexer::wordState() {
    State state = State::Word;
    char ch = chars.peek();
    if (isSpace(ch) || ch == EOF) {
        chars.consume();
        state = State::Empty;
    } else if (isMetaCharacter(ch)) {
        state = State::Operator;
    } else {
        state_data.word.push_back(ch);
        chars.consume();
        return State::Word;
    }

    delimit(Token {
        .type = Token::Type::Word,
        .word_token = WordToken {
            .value = std::move(state_data.word)
        }
    });
    state_data.word.clear();
    return state;
}

Lexer::State Lexer::operatorState() {
    // todo: multiple operator types
    char ch = chars.consume();
    assert(isMetaCharacter(ch));
    delimit(Token {
        .type = Token::Type::Operator,
        .operator_token = {
            .type = OperatorToken::Type::Semicolon
        }
    });
    return State::Empty;
}

// https://www.gnu.org/software/bash/manual/bash.html#index-metacharacter
constexpr const char* WHITESPACE = " \n\t";
// constexpr const char* METACHARACTERS = " \n\t|&;()<>";
constexpr const char* METACHARACTERS = ";";

static bool contains(const char* s, char ch) {
    return strchr(s, ch) != nullptr;
}

bool isSpace(char ch) {
    return contains(WHITESPACE, ch);
}

bool isMetaCharacter(char ch) {
    return contains(METACHARACTERS, ch);
}

}; //namespace shell
