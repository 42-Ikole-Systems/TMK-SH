#pragma once

#include "lexer/token.hpp"
#include "util.hpp"

namespace shell {

class Parser {
private:

public:
	struct AST {};

	AST getNextCommand(vector<Token> tokens);
};

} // namespace shell
