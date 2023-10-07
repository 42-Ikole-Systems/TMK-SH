#pragma once

#include "reader.hpp"
#include "util.hpp"
#include "token.hpp"
#include "interfaces/provider.hpp"
#include <functional>
#include <deque>

namespace shell {

class Lexer : public Provider<optional<Token>> {
public:
	enum State { Empty, Done, Word, Operator };

private:
	struct StateData {
		string word; // is this enough state?
	};

private:
	Provider<char> &chars; // kind of unsafe to only have a reference
	State state;
	StateData state_data;
	optional<Token> token;
	std::deque<Token> tokens;

public:
	Lexer(Provider<char> &chars);
	Lexer(Provider<char> &chars, State initial);

	/**
	 * @brief Lex line into list of tokens
	 *
	 * @param line line to be lexed
	 * @return vector<Token> vector of tokens resulting from lexing
	 */
	optional<Token> peek(size_t n = 0);
	optional<Token> consume(size_t n = 0);
	// vector<Token> tokenize(const string &line);

private:
	void delimit(Token &&token);
	void generateTokens(size_t n);

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
