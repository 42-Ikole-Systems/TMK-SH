#include "lexer/lexer.hpp"
#include <stdexcept>
#include "util.hpp"
#include <stdio.h>
#include "logger.hpp"
#include "assert.hpp"

namespace shell {

Lexer::Lexer(Provider<char> &chars, State initial) : chars(chars), state(initial) {
}
Lexer::Lexer(Provider<char> &chars) : Lexer(chars, State::Empty) {
}

optional<Token> Lexer::peek() {
	if (!token.has_value()) {
		nextToken();
	}
	return token;
}

optional<Token> Lexer::consume() {
	if (!token.has_value()) {
		nextToken();
	}
	// move constructs other if present
	optional<Token> other = nullopt;
	token.swap(other);
	other.value().print();
	return other;
}

/**
 * @brief Generates tokens until tokens.size() >= n or EOF is reached
 *
 * @param n number of tokens to generate
 */
void Lexer::nextToken() {
	while (state != State::Done && !token.has_value()) {
		auto handler = getStateHandler();
		state = handler(*this);
	}
}

void Lexer::delimit(Token &&token) {
	this->token = std::move(token);
}

std::function<Lexer::State(Lexer &)> Lexer::getStateHandler() {
	static const std::function<State(Lexer &)> handlers[] = {
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
	} else if (ch == '\n') {
		delimit(Token {Token::Type::Newline, Newline()});
		chars.consume();
		return State::Empty;
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
		state = State::Empty;
	} else if (isMetaCharacter(ch)) {
		state = State::Operator;
	} else {
		state_data.word.push_back(ch);
		chars.consume();
		return State::Word;
	}

	delimit(Token {Token::Type::Word, WordToken {std::move(state_data.word)}});
	state_data.word.clear();
	return state;
}

Lexer::State Lexer::operatorState() {
	char ch = chars.consume();
	D_ASSERT(isMetaCharacter(ch));
	Token::Type type;
	if (ch == ';') {
		type = Token::Type::Semicolon;
	} else if (ch == '&') {
		type = Token::Type::And;
	} else {
		throw std::runtime_error("not implemented");
	}
	delimit(Token {type, OperatorToken()});
	return State::Empty;
}

// https://www.gnu.org/software/bash/manual/bash.html#index-metacharacter
constexpr const char *WHITESPACE = " \n\t";
// constexpr const char* METACHARACTERS = " \n\t|&;()<>";
constexpr const char *METACHARACTERS = ";&";

static bool contains(const char *s, char ch) {
	return strchr(s, ch) != nullptr;
}

bool isSpace(char ch) {
	return contains(WHITESPACE, ch);
}

bool isMetaCharacter(char ch) {
	return contains(METACHARACTERS, ch);
}

}; // namespace shell
