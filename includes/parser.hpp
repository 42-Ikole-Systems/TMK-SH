#pragma once

#include "interfaces/provider.hpp"
#include "lexer/token.hpp"
#include "util.hpp"
#include "ast.hpp"

namespace shell {

class Parser {
private:
	Provider<optional<Token>> &tokens;
	vector<Token> fetched;
	size_t token_position; // token position state

public:
	Parser(Provider<optional<Token>> &tokens);

	Ast getNextCommand();

private:
	bool retrieveTokens(size_t n);

	// should probably return some optional of the variant type... (also node should be a variant and not a base class)
	optional<Token> peekToken(size_t n = 0);
	optional<Token> consumeToken(size_t n = 0);
	size_t saveState();
	void setState(size_t state);
	size_t unconsumedTokens();

	optional<Ast::Node> parseList();
	optional<Ast::SeparatorOp> parseSeparatorOp();
	optional<Ast::Command> parseCommand();
};

} // namespace shell
