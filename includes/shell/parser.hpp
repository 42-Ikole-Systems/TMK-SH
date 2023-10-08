#pragma once

#include "shell/lexer/token_provider.hpp"
#include "shell/lexer/token.hpp"
#include "shell/util.hpp"
#include "shell/ast.hpp"

namespace shell {

class Parser {
private:
	TokenProvider &tokens;
	vector<Token> fetched;
	size_t token_position; // token position state

public:
	Parser(TokenProvider &tokens);

	Ast getNextCommand();

private:
	bool nextToken();

	optional<Token> peekToken();
	optional<Token> consumeToken();
	size_t saveState();
	void setState(size_t state);
	size_t unconsumedTokens();

	optional<Ast::Node> parseList();
	optional<Ast::SeparatorOp> parseSeparatorOp();
	optional<Ast::Command> parseCommand();
};

} // namespace shell
