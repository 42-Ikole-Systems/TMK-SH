#pragma once

#include "shell/interfaces/reader.hpp"
#include "shell/utility/types.hpp"
#include "shell/lexer/token.hpp"
#include "shell/lexer/token_provider.hpp"
#include "shell/interfaces/provider.hpp"
#include <functional>
#include <stack>

namespace shell {

class Lexer : public TokenProvider {
public:
	enum class State : uint8_t {
		Empty,
		Done,
		Word,
		Operator,
		Comment,
		Backslash,
		SingleQuote,
		DoubleQuote,
		InnerBackslash,
		ExpansionStart,
		ParameterExpansion,
		CommandSubstitution,
		BackQuote,
		ArithmeticExpansion
	};

private:
	struct StateData {
		string word; // is this enough state?
		std::stack<State> states;

		void pushState(State state);
		State popState();
		State previousState();
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

	State getCurrentState() const;

private:
	void delimit(Token &&token);
	void nextToken();
	State nextNestedState(State current, char ch);
	State generalExpansionHandler(State state, char terminator);

	void delimitOperator();

	std::function<State(Lexer &)> getStateHandler();
	State emptyState();
	State wordState();
	State operatorState();
	State commentState();
	State backslashState();
	State singleQuoteState();
	State doubleQuoteState();
	State innerBackslashState();
	State expansionStartState();
	State parameterExpansionState();
	State commandSubstitutionState();
	State arithmeticExpansionState();
	State backQuoteState();
};

/*
Utility functions for lexing
 */

// todo: inline (?)
bool isSpace(char ch);
bool isMetaCharacter(char ch);
bool isOperatorCharacter(char ch);
bool isOperatorStart(char ch);
bool isCommentStart(char ch);
bool isNewline(char ch);
bool isBackslash(char ch);
bool isSingleQuote(char ch);
bool isDoubleQuote(char ch);
bool isDollarSign(char ch);
bool isBackQuote(char ch);

} // namespace shell
