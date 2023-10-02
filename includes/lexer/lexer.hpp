#pragma once

#include "reader.hpp"
#include "util.hpp"
#include "token.hpp"
#include "interfaces/provider.hpp"
#include <functional>
#include <queue>

namespace shell {

class Lexer: public Provider<optional<Token>> {
public:
	enum State { Empty, Done, Word, Operator };

private:
	struct StateData {
		string word; // is this enough state?
	};

private:
	// Reader &reader;
	Provider<char>& chars; // kind of unsafe to only have a reference
	State state;
	StateData state_data;
	optional<Token> token;
	std::queue<Token> tokens;

public:
	Lexer(Provider<char>& chars);
	Lexer(Provider<char>& chars, State initial);

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
	void delimit(Token&& token);
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
