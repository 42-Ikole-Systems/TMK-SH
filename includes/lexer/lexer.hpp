#pragma once

#include "reader.hpp"
#include "util.hpp"
#include "token.hpp"
#include <functional>
#include <queue>

namespace shell {

class CharProvider {
public:
	virtual ~CharProvider() {
	}
	// EOF when there are no chars left to provide
	virtual char peek() = 0;
	virtual char consume() = 0;
};

class Lexer {
public:
	enum State { Empty, Done, Word, Operator };

private:
	struct StateData {
		// optional<Token> nextToken;
		// vector<Token> tokens;
		std::queue<Token> tokens;
		string word; // is this enough state?
		// CharProvider &chars;
	};

private:
	// Reader &reader;
	CharProvider& chars; // kind of unsafe to only have a reference
	State state;
	StateData state_data;
	optional<Token> token;

public:
	Lexer(CharProvider& chars);
	Lexer(CharProvider& chars, State state);

	/**
	 * @brief Lex line into list of tokens
	 *
	 * @param line line to be lexed
	 * @return vector<Token> vector of tokens resulting from lexing
	 */
	optional<Token> peek();
	optional<Token> consume();
	// vector<Token> tokenize(const string &line);

private:
	optional<Token> nextToken();
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

} // namespace shell
