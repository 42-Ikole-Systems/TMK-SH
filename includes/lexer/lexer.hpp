#pragma once

#include "reader.hpp"
#include "util.hpp"
#include "token.hpp"
#include <functional>

namespace shell {

class CharProvider {
public:
	virtual ~CharProvider() {
	}
	virtual char peek() = 0;
	virtual char consume() = 0;
};

class Lexer {
private:
	Reader &reader;

	enum State { Empty, Done, Word, Operator };

	struct StateData {
		StateData(CharProvider &provider);

		vector<Token> tokens;
		string word;
		CharProvider &chars;
	};

public:
	Lexer(Reader &reader);

	/**
	 * @brief Lex line into list of tokens
	 *
	 * @param line line to be lexed
	 * @return vector<Token> vector of tokens resulting from lexing
	 */
	vector<Token> tokenize(const string &line);

private:
	std::function<State(Lexer &, StateData &)> getStateHandler(State state);
	State emptyState(StateData &data);
	State wordState(StateData &data);
	State operatorState(StateData &data);
};

/*
Utility functions for lexing
 */

bool isSpace(char ch);
bool isMetaCharacter(char ch);

} // namespace shell
