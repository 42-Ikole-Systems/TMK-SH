#pragma once

#include "reader.hpp"
#include "util.hpp"
#include "token_parser.hpp"
#include "token.hpp"

namespace shell {

class Lexer {
private:
	Reader& reader;
	unique_ptr<TokenParser> token_parser;

public:
	Lexer(Reader& reader);

	/**
	 * @brief Get the Next Token object
	 * May read
	 * @return Token
	 */
	optional<Token> getNextToken();

	/**
	 * @brief Lex line into list of tokens
	 * 
	 * @param line line to be lexed
	 * @return vector<Token> vector of tokens resulting from lexing
	 */
	vector<Token> lexTokens(const string& line);

private:
	char nextChar();
};

} // namespace shell
