#pragma once

#include "interfaces/provider.hpp"
#include "lexer/token.hpp"
#include "util.hpp"
#include "ast.hpp"

namespace shell {

class Parser {
private:
	Provider<optional<Token>>& tokens;

public:
	Parser(Provider<optional<Token>>& tokens);

	Ast getNextCommand();
};

} // namespace shell
