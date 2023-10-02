#include "lexer/lexer.hpp"
#include <stdexcept>
#include "util.hpp"
#include <stdio.h>
#include <cassert>
#include "logger.hpp"

namespace shell {

Lexer::Lexer(Provider<char>& chars, State initial) : chars(chars), state(initial) {}
Lexer::Lexer(Provider<char>& chars) : Lexer(chars, State::Empty) {}

optional<Token> Lexer::peek(size_t n) {
    generateTokens(n + 1);
    if (tokens.size() < n + 1) {
        return nullopt;
    }
    return tokens[n];
}

// consume n tokens and return the last
optional<Token> Lexer::consume(size_t n) {
    generateTokens(n + 1);
    if (tokens.size() < n + 1) {
        return nullopt;
    }
    tokens.erase(tokens.begin(), tokens.begin() + n);
    Token token = tokens.front();
    tokens.pop_front();
    return token;
}

/**
 * @brief Generates tokens until tokens.size() >= n or EOF is reached
 * 
 * @param n number of tokens to generate
 */
void Lexer::generateTokens(size_t n) {
    while (state != State::Done && tokens.size() < n) {
        auto handler = getStateHandler();
        state = handler(*this);
    }
}

void Lexer::delimit(Token&& token) {
    tokens.emplace_back(token);
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
        .operator_token = OperatorToken(ch)
    });
    return State::Empty;
}

// https://www.gnu.org/software/bash/manual/bash.html#index-metacharacter
constexpr const char* WHITESPACE = " \n\t";
// constexpr const char* METACHARACTERS = " \n\t|&;()<>";
constexpr const char* METACHARACTERS = ";&";

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
