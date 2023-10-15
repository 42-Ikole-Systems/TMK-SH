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

Lexer::State Lexer::getCurrentState() const {
	return state;
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

/*
Handler requirements:
	- Delimits at most one token in a single run
	- Returns the next state in case of a state transition requirement
Handlers are free to loop and process as many chars as necessary in order to delimit or encounter a state transition.
*/
std::function<Lexer::State(Lexer &)> Lexer::getStateHandler() {
	static const std::function<State(Lexer &)> handlers[] = {
	    [(int)Lexer::State::Empty] = &Lexer::emptyState,
	    [(int)Lexer::State::Word] = &Lexer::wordState,
	    [(int)Lexer::State::Operator] = &Lexer::operatorState,
	    [(int)Lexer::State::Comment] = &Lexer::commentState,
	    [(int)Lexer::State::Backslash] = &Lexer::backslashState,
	    [(int)Lexer::State::SingleQuote] = &Lexer::singleQuoteState,
	    [(int)Lexer::State::DoubleQuote] = &Lexer::doubleQuoteState,
	    [(int)Lexer::State::InnerBackslash] = &Lexer::innerBackslashState,
	    [(int)Lexer::State::ExpansionStart] = &Lexer::expansionStartState,
	    [(int)Lexer::State::BackTick] = &Lexer::backTickState,
	    [(int)Lexer::State::ParameterExpansion] = &Lexer::parameterExpansionState,
	    [(int)Lexer::State::ArithmeticExpansion] = &Lexer::arithmeticExpansionState,
	    [(int)Lexer::State::CommandSubstitution] = &Lexer::commandSubstitutionState,
	};
	D_ASSERT((size_t)state < (sizeof(handlers) / sizeof(handlers[0])));
	D_ASSERT(handlers[(int)state] != nullptr);
	return handlers[((int)state)];
}

Lexer::State Lexer::emptyState() {
	char ch = chars.peek();
	while (true) {
		char ch = chars.peek();
		if (ch == EOF) {
			return State::Done;
		} else if (ch == '\n') {
			delimit(Token {Token::Type::Newline, Newline()});
			chars.consume();
			return State::Empty;
		} else if (isSpace(ch)) {
			chars.consume();
		} else if (isOperatorStart(ch)) {
			return State::Operator;
		} else {
			return State::Word;
		}
	}
}

/*
Operator -> delimit + switch state
Backslash -> push state + switch state
(D|S)Quote/Expansion/Backtick -> pushChar + consume + pushState + switch state
Space || EOF -> delimit + switch to Empty
Comment -> delimit + switch to Comment
*/
Lexer::State Lexer::wordState() {
	State state = State::Word;
	while (state == State::Word) {
		char ch = chars.peek();

		// [ push char, consume, push state ], switch state
		if (isSingleQuote(ch) || isDoubleQuote(ch) || isDollarSign(ch) || isBackTick(ch)) {
			state_data.word.push_back(chars.consume());
			state_data.pushState(State::Word);
			if (isSingleQuote(ch)) {
				return State::SingleQuote;
			} else if (isDoubleQuote(ch)) {
				return State::DoubleQuote;
			} else if (isDollarSign(ch)) {
				return State::ExpansionStart;
			} else if (isBackTick(ch)) {
				return State::BackTick;
			}
		}
		// [ push state, switch state ]
		if (isBackslash(ch)) {
			state_data.pushState(State::Word);
			return State::Backslash;
		}

		// [ delimit ], switch state
		if (isSpace(ch) || ch == EOF) {
			state = State::Empty;
		} else if (isCommentStart(ch)) {
			state = State::Comment;
		} else if (isOperatorStart(ch)) {
			state = State::Operator;
		} else {
			// Continue word state
			state_data.word.push_back(chars.consume());
		}
	}
	// Handle delimit outside of loop: EOF, IFS, operator, comment
	if (!state_data.word.empty()) {
		delimit(Token{Token::Type::Word, WordToken{std::move(state_data.word)}});
		state_data.word.clear();
	}
	return state;
}

Lexer::State Lexer::operatorState() {
	char ch = chars.peek();

	if (isBackslash(ch)) {
		state_data.pushState(State::Operator);
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

// Precondition: backslash is NOT consumed. Previous state is either WORD or OPERATOR
Lexer::State Lexer::backslashState() {
	char ch = chars.consume();
	D_ASSERT(isBackslash(ch));
	ch = chars.peek();
	if (ch == EOF) {
		// todo: determine behavior
		// throw std::runtime_error("encountered EOF in backslash lexing");
		return state_data.popState();
	}
	if (isNewline(ch)) {
		// Special case: `\\n`: remove backslash and newline from the input stream and continue where we left off.
		chars.unconsume();
		// Todo: if the NEXT character IS another newline, then the backslash _IS_ added to the history. Maybe just a
		// bash/zsh behavior?
		chars.remove();
		chars.remove();
		return state_data.popState();
	}

	// This is required to be here in the case of something like `>\\n>`, which results in the next token being used as
	// an operator. Since we have not encountered a newline, that scenario is no longer possible and the literal value
	// is used as part of a word, so we delimit the operator at this point.
	if (state_data.previousState() == State::Operator) {
		delimitOperator();
	}

	state_data.word.push_back('\\');
	state_data.word.push_back(ch);
	chars.consume();
	return state_data.popState();
}

/*
Preconditions:
	- Starting single quote is consumed
	- Previous state is on the stack
No transitions, everything is literal.
*/
Lexer::State Lexer::singleQuoteState() {
	while (true) {
		char ch = chars.consume();
		if (ch == EOF) {
			// todo: determine behavior
			// throw std::runtime_error("encountered EOF in single quote parsing");
			return state_data.popState();
		}
		state_data.word.push_back(ch);
		if (isSingleQuote(ch)) {
			return state_data.popState();
		}
	}
}

/*
Preconditions:
	- Starting double quote is consumed
	- Previous state is on the stack
Nested transitions: \, `, $ (expansions)
*/
Lexer::State Lexer::doubleQuoteState() {
	auto state = State::DoubleQuote;
	while (state == State::DoubleQuote) {
		char ch = chars.consume();
		if (ch == EOF) {
			// todo: determine behavior
			// throw std::runtime_error("encountered EOF in double quote lexing");
			return state_data.popState();
		}
		state_data.word.push_back(ch);
		if (isDoubleQuote(ch)) {
			return state_data.popState();
		}
		if (isBackslash(ch)) {
			state = State::InnerBackslash;
		} else if (isBackTick(ch)) {
			state = State::BackTick;
		} else if (isDollarSign(ch)) {
			state = State::ExpansionStart;
		}
	}
	// Nested state transition
	state_data.pushState(State::DoubleQuote);
	return state;
}

// preconditions:
//	- backslash is already consumed and added to word
//	- previous state is on the stack
// Does NOT remove <newline> from the input stream
Lexer::State Lexer::innerBackslashState() {
	char ch = chars.consume();
	if (ch == EOF) {
		// todo: determine behavior
		// throw std::runtime_error("encountered EOF in double quote backslash lexing");
		return state_data.popState();
	}
	state_data.word.push_back(ch);
	return state_data.popState();
}

Lexer::State Lexer::expansionStartState() {
	switch (chars.peek()) {
		case '{':
			state_data.word.push_back(chars.consume());
			return State::ParameterExpansion;
		case '(':
			state_data.word.push_back(chars.consume());
			if (chars.peek() == '(') {
				state_data.word.push_back(chars.consume());
				return State::ArithmeticExpansion;
			} else {
				return State::CommandSubstitution;
			}
		default:
			return state_data.popState();
	}
}

// same as double quote, except it can also transition to single quote
Lexer::State Lexer::nextNestedState(State current, char ch) {
	if (ch == EOF) {
		throw std::runtime_error("unexpected EOF in nested state");
	} else if (isBackslash(ch)) {
		return State::InnerBackslash;
	} else if (isDoubleQuote(ch)) {
		return State::DoubleQuote;
	} else if (isSingleQuote(ch)) {
		return State::SingleQuote;
	} else if (isDollarSign(ch)) {
		return State::ExpansionStart;
	} else if (isBackTick(ch)) {
		return State::BackTick;
	}
	return current;
}

Lexer::State Lexer::generalExpansionHandler(State state, char terminator) {
	auto current = state;
	while (current == state) {
		char ch = chars.consume();
		if (ch == EOF) {
			return state_data.popState();
		}
		state_data.word.push_back(ch);
		if (ch == terminator) {
			return state_data.popState();
		}
		current = nextNestedState(current, ch);
	}
	state_data.pushState(state);
	return current;
}


Lexer::State Lexer::parameterExpansionState() {
	return generalExpansionHandler(state, '}');
}

Lexer::State Lexer::commandSubstitutionState() {
	return generalExpansionHandler(state, ')');
}

// Requires some extra logic due to having multiple terminating characters
Lexer::State Lexer::arithmeticExpansionState() {
	auto state = State::ArithmeticExpansion;
	while (state == State::ArithmeticExpansion) {
		char ch = chars.consume();
		if (ch == EOF) {
			return state_data.popState();
		}
		state_data.word.push_back(ch);
		if (ch == ')') {
			if (chars.peek() == ')') {
				state_data.word.push_back(chars.consume());
				return state_data.popState();
			} else {
				// If it closes with ')' instead of '))', we are actually in a subshell of a command substitution
				// Example: $((echo ...) | cat)
				return State::CommandSubstitution;
			}
		}
		state = nextNestedState(State::ArithmeticExpansion, ch);
	}
	state_data.pushState(State::ArithmeticExpansion);
	return state;
}

Lexer::State Lexer::backTickState() {
	while (true) {
		char ch = chars.consume();
		state_data.word.push_back(ch);
		if (isBackslash(ch)) {
			state_data.pushState(State::BackTick);
			return State::InnerBackslash;
		} else if (isBackTick(ch)) {
			return state_data.popState();
		}
	}
}

/* State Data methods */

void Lexer::StateData::pushState(State state) {
	states.push(state);
}

Lexer::State Lexer::StateData::popState() {
	D_ASSERT(!states.empty());
	State state = states.top();
	states.pop();
	return state;
}

Lexer::State Lexer::StateData::previousState() {
	D_ASSERT(!states.empty());
	return states.top();
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
