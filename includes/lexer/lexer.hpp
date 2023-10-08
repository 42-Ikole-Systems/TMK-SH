#pragma once

#include "reader.hpp"
#include "util.hpp"
#include "token.hpp"
#include "token_provider.hpp"
#include "interfaces/provider.hpp"
#include <functional>
#include <deque>

namespace shell {

class Lexer : public TokenProvider {
private:
	enum State { Empty, Done, Word, Operator };

	struct StateData {
		string word; // is this enough state?
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

	std::function<State(Lexer &)> getStateHandler();
	State emptyState();
	State wordState();
	State operatorState();
};

/*
Utility functions for lexing
 */

bool isSpace(char ch);
bool isMetaCharacter(char ch);
bool isOperatorCharacter(char ch);

} // namespace shell
