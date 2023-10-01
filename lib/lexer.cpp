#include "lexer/lexer.hpp"
#include <stdexcept>
#include "util.hpp"
#include <stdio.h>
#include <cassert>

namespace shell {

Lexer::Lexer(Reader& reader) : reader(reader) {}

Lexer::StateData::StateData(CharProvider& chars): chars(chars) {}

class LineCharProvider: public CharProvider {
private:
    string line;
    size_t index;
public:
    LineCharProvider(const string& line): line(line), index(0) {}

    char peek() override {
        if (index >= line.length()) {
            return EOF;
        }
        return line[index];
    }

    char consume() override {
        if (index >= line.length()) {
            return EOF;
        }
        return line[index++];
    }
};

vector<Token> Lexer::tokenize(const string& line) {
    auto chars = LineCharProvider(line);
    auto state_data = StateData(chars);
    State state = State::Empty;
    while (state != State::Done) {
        auto handler = getStateHandler(state);
        state = handler(*this, state_data);
    }
    return state_data.tokens;
}

std::function<Lexer::State(Lexer&, Lexer::StateData&)> Lexer::getStateHandler(State state) {
    static const std::function<State(Lexer&, StateData&)> handlers[] = {
        [Empty] = &Lexer::emptyState,
        [Word] = &Lexer::wordState,
        [Operator] = &Lexer::operatorState,
    };
    return handlers[state];
}

Lexer::State Lexer::emptyState(StateData& data) {
    char ch = data.chars.peek();
    if (ch == EOF) {
        return State::Done;
    } else if (isSpace(ch)) {
        data.chars.consume();
        return State::Empty;
    } else if (isMetaCharacter(ch)) {
        return State::Operator;
    } else {
        return State::Word;
    }
}

Lexer::State Lexer::wordState(StateData& data) {
    State state = State::Word;
    char ch = data.chars.peek();
    if (isSpace(ch) || ch == EOF) {
        data.chars.consume();
        state = State::Empty;
    } else if (isMetaCharacter(ch)) {
        state = State::Operator;
    } else {
        data.word.push_back(data.chars.consume());
        return State::Word;
    }

    data.tokens.push_back(Token {
        .type = Token::Type::Word,
        .word_token = WordToken {
            .value = std::move(data.word)
        }
    });
    data.word.clear();
    return state;
}

Lexer::State Lexer::operatorState(StateData& data) {
    // todo: multiple operator types
    char ch = data.chars.consume();
    assert(isMetaCharacter(ch));
    data.tokens.push_back(Token {
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
