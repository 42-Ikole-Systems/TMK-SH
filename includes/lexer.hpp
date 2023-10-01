#pragma once

#include "reader.hpp"
#include "util.hpp"

namespace shell {

class Lexer {
private:
	unique_ptr<Reader> reader;

public:
	Lexer(unique_ptr<Reader> reader);
	Lexer(Lexer &&other);

	struct Token {};

	/**
	 * @brief Get the Next Token object
	 * May read
	 * @return Token
	 */
	Token getNextToken();
};

} // namespace shell
