#pragma once

#include "shell/interfaces/reader.hpp"
#include "shell/util.hpp"
#include "shell/lexer/token.hpp"
#include "shell/lexer/token_provider.hpp"
#include "shell/interfaces/provider.hpp"
#include <functional>
#include <deque>

namespace shell {

class Lexer : public TokenProvider {
private:
	enum class State : uint8_t {
		Empty,
		Done,
		Word,
		Operator,
		Comment,
		Backslash,
		SingleQuoteStart,
		SingleQuote,
	};

	struct StateData {
		string word;    // is this enough state?
		State previous; // will probably need a stack for expansion lexing?
	};

private:
	Provider<char> &chars; // kind of unsafe to only have a reference
	State state;
	StateData state_data;
	optional<Token> token;

public:
	Lexer(Provider<char> &chars);
	Lexer(Provider<char> &chars, State initial);

	optional<Token> peek() override;
	optional<Token> consume() override;

private:
	void delimit(Token &&token);
	void nextToken();

	void delimitOperator();

	std::function<State(Lexer &)> getStateHandler();
	State emptyState();
	State wordState();
	State operatorState();
	State commentState();
	State backslashState();
	State singleQuoteStartState();
	State singleQuoteState();
};

/*
Utility functions for lexing
 */

// todo: inline
bool isSpace(char ch);
bool isMetaCharacter(char ch);
bool isOperatorCharacter(char ch);
bool isOperatorStart(char ch);
bool isCommentStart(char ch);
bool isNewline(char ch);
bool isBackslash(char ch);
bool isSingleQuote(char ch);
bool isDoubleQuote(char ch);

} // namespace shell
