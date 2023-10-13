#include "shell/lexer/lexer.hpp"
#include "shell/util.hpp"
#include <stdio.h>
#include "shell/logger.hpp"
#include "shell/assert.hpp"
#include "shell/exception.hpp"

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
		LOG_DEBUG("%\n", other.value());
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
	    [(int)Lexer::State::Comment] = &Lexer::commentState,
	    [(int)Lexer::State::Backslash] = &Lexer::backslashState,
	    [(int)Lexer::State::SingleQuoteStart] = &Lexer::singleQuoteStartState,
	    [(int)Lexer::State::SingleQuote] = &Lexer::singleQuoteState,
	    [(int)Lexer::State::DoubleQuoteStart] = &Lexer::doubleQuoteStateStart,
	    [(int)Lexer::State::DoubleQuote] = &Lexer::doubleQuoteState,
	    [(int)Lexer::State::DoubleQuoteBackslash] = &Lexer::doubleQuoteBackslashState,
	    [(int)Lexer::State::ExpansionStart] = &Lexer::expansionStartState,
	    [(int)Lexer::State::ParameterExpansion] = &Lexer::parameterExpansionState,
	};
	D_ASSERT((size_t)state < (sizeof(handlers) / sizeof(handlers[0])));
	D_ASSERT(handlers[(int)state] != nullptr);
	return handlers[((int)state)];
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
	} else if (isOperatorStart(ch)) {
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
	} else if (isCommentStart(ch)) {
		state = State::Comment;
	} else if (isOperatorStart(ch)) {
		state = State::Operator;
	} else if (isCommentStart(ch)) {
		state = State::Comment;
	} else if (isBackslash(ch)) {
		state_data.previous = State::Word;
		return State::Backslash;
	} else if (isSingleQuote(ch)) {
		return State::SingleQuoteStart;
	} else if (isDoubleQuote(ch)) {
		return State::DoubleQuoteStart;
	} else if (ch == '$') {
		return State::ExpansionStart;
	} else if (ch == '`') {
		throw NotImplementedException();
		// state_data.word.push_back(chars.consume());
		// return State::CommandSubstitution;
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

	if (isBackslash(ch)) {
		state_data.previous = State::Operator;
		return State::Backslash;
	}

	int matches;
	if (!isOperatorCharacter(ch)) {
		matches = 0;
	} else {
		matches = Token::prefixOperatorMatches(state_data.word + ch);
	}

	if (matches >= 1) {
		// The characters we have now are part of 1 or multiple operators
		// don't delimit yet, because this does not tell us that it matches an entire operator
		state_data.word.push_back(ch);
		chars.consume();
		return State::Operator;
	}
	delimitOperator();
	return State::Empty;
}

// Previously it did match a prefix of a valid operator
// but with the additional character it doesn't;
// - find out what the previous operator was
void Lexer::delimitOperator() {
	auto result = Token::exactOperatorType(state_data.word);
	D_ASSERT(result.has_value());
	delimit(Token {result.value(), OperatorToken()});
	state_data.word.clear();
}

Lexer::State Lexer::commentState() {
	char ch = chars.peek();
	if (isNewline(ch) || ch == EOF) {
		return State::Empty;
	}
	chars.consume();
	return State::Comment;
}

Lexer::State Lexer::backslashState() {
	char ch = chars.consume();
	D_ASSERT(isBackslash(ch));
	ch = chars.peek();
	if (ch == EOF) {
		// todo: determine behavior
		throw std::runtime_error("encountered EOF in backslash lexing");
	}
	if (isNewline(ch)) {
		// Special case: `\\n`: remove backslash and newline from the input stream and continue where we left off.
		chars.unconsume();
		// Todo: if the NEXT character IS another newline, then the backslash _IS_ added to the history. Maybe just a
		// bash/zsh behavior?
		chars.remove();
		chars.remove();
		return state_data.previous;
	}

	// This is required to be here in the case of something like `>\\n>`, which results in the next token being used as
	// an operator. Since we have not encountered a newline, that scenario is no longer possible and the literal value
	// is used as part of a word, so we delimit the operator at this point.
	if (state_data.previous == State::Operator) {
		delimitOperator();
	}

	state_data.word.push_back('\\');
	state_data.word.push_back(ch);
	chars.consume();
	return State::Word;
}

Lexer::State Lexer::singleQuoteStartState() {
	char ch = chars.consume();
	D_ASSERT(isSingleQuote(ch));
	state_data.word.push_back(ch);
	return State::SingleQuote;
}

Lexer::State Lexer::singleQuoteState() {
	char ch = chars.consume();
	if (ch == EOF) {
		// todo: determine behavior
		throw std::runtime_error("encountered EOF in single quote parsing");
	}
	state_data.word.push_back(ch);
	if (isSingleQuote(ch)) {
		return State::Word;
	}
	return State::SingleQuote;
}

Lexer::State Lexer::doubleQuoteStateStart() {
	char ch = chars.consume();
	D_ASSERT(isDoubleQuote(ch));
	state_data.word.push_back(ch);
	return State::DoubleQuote;
}

Lexer::State Lexer::doubleQuoteState() {
	char ch = chars.consume();
	if (ch == EOF) {
		// todo: determine behavior
		throw std::runtime_error("encountered EOF in double quote lexing");
	}
	if (isBackslash(ch)) {
		// May not push back backslash in case of new line
		return State::DoubleQuoteBackslash;
	}
	state_data.word.push_back(ch);
	if (isDoubleQuote(ch)) {
		return State::Word;
	} else if (isBackTick(ch)) {
		throw NotImplementedException();
	} else if (isDollarSign(ch)) {
		throw NotImplementedException();
	}
	return State::DoubleQuote;
}

// precondition: backslash is already consumed
// separate state since nothing is actually removed from the input stream here (and it only works for certain
// characters)
Lexer::State Lexer::doubleQuoteBackslashState() {
	char ch = chars.consume();
	if (ch == EOF) {
		// todo: determine behavior
		throw std::runtime_error("encountered EOF in double quote backslash lexing");
	}
	if (isNewline(ch)) {
		return State::DoubleQuote;
	}
	state_data.word.push_back('\\');
	state_data.word.push_back(ch);
	return State::DoubleQuote;
}

Lexer::State Lexer::expansionStartState() {
	char ch = chars.consume();
	D_ASSERT(ch == '$');
	state_data.word.push_back(ch);
	ch = chars.peek();
	state_data.word.push_back(ch);
	switch (ch) {
		case '{':
			state_data.word.push_back(chars.consume());
			return State::ParameterExpansion;
		case '(':
			throw NotImplementedException();
			// return CommandSubstitutionStart;
			// state_data.word.push_back(chars.consume());
			// if (chars.peek() == '(') {
			// 	state_data.word.push_back(chars.consume());
			// 	return State::ArithmeticExpansion;
			// } else {
			// 	return State::CommandSubstitution;
			// }
		default:
			// Reasoning: parameter expansion without special behavior
			return State::Word;
	}
}

Lexer::State Lexer::parameterExpansionState() {
	char ch = chars.peek();
	// Quote: Double and Single (same behavior)
	// Backslash (same behavior)
	// CommandSubstitution (same behavior)
	// Nested ParameterExpansion (same behavior)
	// ArithmeticExpansion (same behavior)
	throw NotImplementedException();
}

// https://www.gnu.org/software/bash/manual/bash.html#index-metacharacter
#define WHITESPACE                " \t\n"
#define OPERATOR_START_CHARACTERS "|&;()<>"                            // characters that operators can start with
#define OPERATOR_CHARACTERS       OPERATOR_START_CHARACTERS "-"        // characters that _can_ appear in operators
#define METACHARACTERS            WHITESPACE OPERATOR_START_CHARACTERS // characters that _always_ delimit words

static bool contains(const char *s, char ch) {
	return strchr(s, ch) != nullptr;
}

bool isCommentStart(char ch) {
	return ch == '#';
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

bool isOperatorStart(char ch) {
	return contains(OPERATOR_START_CHARACTERS, ch);
}

bool isNewline(char ch) {
	return ch == '\n';
}

bool isBackslash(char ch) {
	return ch == '\\';
}

bool isSingleQuote(char ch) {
	return ch == '\'';
}

bool isDoubleQuote(char ch) {
	return ch == '\"';
}

bool isDollarSign(char ch) {
	return ch == '$';
}

bool isBackTick(char ch) {
	return ch == '`';
}

}; // namespace shell
