#pragma once

#include "lexer/token_provider.hpp"
#include "lexer/token.hpp"
#include "util.hpp"
#include "ast.hpp"
#include "grammar/rule.hpp"

namespace shell {

class BufferedTokenProvider;

class Parser {
private:
	friend class BufferedTokenProvider;

private:
	TokenProvider &tokens;
	vector<Token> fetched;
	size_t token_position; // token position state

public:
	Parser(TokenProvider &tokens);
	Ast parse();

private:
	bool nextToken();

	optional<Token> peekToken();
	optional<Token> consumeToken();
	size_t saveState();
	void setState(size_t state);
	size_t unconsumedTokens();

	optional<Ast::Node> parse(Rule rule);
	optional<Ast::Node> parse(Rule::NonTerminal &non_terminal);
	optional<Ast::Node> parse(Rule::Terminal &terminal);
	BufferedTokenProvider tokenProvider();
};

class BufferedTokenProvider : public TokenProvider {
public:
	BufferedTokenProvider(Parser &parser) : parser(parser) {
	}
	~BufferedTokenProvider() {
	}

public:
	virtual optional<Token> peek();
	virtual optional<Token> consume();

private:
	Parser &parser;
};

} // namespace shell
