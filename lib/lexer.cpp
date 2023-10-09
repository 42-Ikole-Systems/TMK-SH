#include "shell/lexer/lexer.hpp"
#include <stdexcept>
#include "shell/utility/types.hpp"
#include <stdio.h>
#include "shell/logger.hpp"
#include "shell/assert.hpp"

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
	if (other.has_value()) {
		other.value().print();
	}
	return other;
}

void Lexer::nextToken() {
	while (state != State::Done && !token.has_value()) {
		auto handler = getStateHandler();
		state = handler(*this);
	}
}

void Lexer::delimit(Token &&token) {
	D_ASSERT(!this->token.has_value());
	this->token = std::move(token);
}

std::function<Lexer::State(Lexer &)> Lexer::getStateHandler() {
	static const std::function<State(Lexer &)> handlers[] = {
	    [(int)Lexer::State::Empty] = &Lexer::emptyState,
	    [(int)Lexer::State::Word] = &Lexer::wordState,
	    [(int)Lexer::State::Operator] = &Lexer::operatorState,
	};
	return handlers[(int)state];
}

Lexer::State Lexer::emptyState() {
	char ch = chars.peek();
	if (ch == EOF) {
		return State::Done;
	} else if (ch == '\n') { // todo: carriage return support (store state)
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

	if (!state_data.word.empty()) {
		delimit(Token {Token::Type::Word, WordToken {std::move(state_data.word)}});
	}
	state_data.word.clear();
	return state;
}

Lexer::State Lexer::operatorState() {
	char ch = chars.peek();
	int matches = Token::prefixOperatorMatches(state_data.word + ch);
	if (matches >= 1) {
		// The characters we have now are part of 1 or multiple operators
		// don't delimit yet, because this does not tell us that it matches an entire operator
		state_data.word.push_back(ch);
		chars.consume();
		return State::Operator;
	}

	// Previously it did match a prefix of a valid operator
	// but with the additional character it doesn't;
	// - find out what the previous operator was

	auto result = Token::exactOperatorType(state_data.word);
	D_ASSERT(result.has_value());
	delimit(Token {result.value(), OperatorToken()});
	state_data.word.clear();
	return State::Empty;
}

// https://www.gnu.org/software/bash/manual/bash.html#index-metacharacter
#define WHITESPACE          " \t\n"
#define OPERATOR_CHARACTERS "|&;()<>-"           // characters that _can_ appear in operators
#define METACHARACTERS      WHITESPACE "|&;()<>" // characters that _always_ delimit words

static bool contains(const char *s, char ch) {
	return strchr(s, ch) != nullptr;
}

bool isSpace(char ch) {
	return contains(WHITESPACE, ch);
}

bool isMetaCharacter(char ch) {
	return contains(METACHARACTERS, ch);
}

bool isOperatorCharacter(char ch) {
	return contains(OPERATOR_CHARACTERS, ch);
}

}; // namespace shell
