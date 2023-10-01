#pragma once

#include "lexer.hpp"

namespace shell {

class Parser {
private:
	Lexer lexer;

public:
	struct AST {};

	Parser(Lexer &&lexer);

	AST getNextCommand();
};

} // namespace shell
