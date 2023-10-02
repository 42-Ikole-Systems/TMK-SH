#pragma once

#include "interfaces/provider.hpp"
#include "lexer/token.hpp"
#include "util.hpp"
#include "ast.hpp"

namespace shell {

class Parser {
private:
	Provider<optional<Token>>& tokens;
	vector<Token> fetched;
	size_t token_position; // token position state
	unique_ptr<Ast::Node> parsed;

public:
	Parser(Provider<optional<Token>>& tokens);

	Ast getNextCommand();

private:
	bool retrieveTokens(size_t n);
	optional<Token> peekToken(size_t n = 0);
	optional<Token> consumeToken(size_t n = 0);
	size_t saveState();
	void setState(size_t state);

	void setParsedNode(unique_ptr<Ast::Node> node);

	bool parseList();
	bool parseSeparatorOp();
	bool parseCommand();

};

} // namespace shell
