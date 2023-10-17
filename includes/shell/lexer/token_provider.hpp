#pragma once

#include "shell/lexer/token.hpp"
#include "shell/utility/types.hpp"

namespace shell {

class TokenProvider {
public:
	virtual ~TokenProvider() {
	}

	virtual optional<Token> peek() = 0;
	virtual optional<Token> consume() = 0;

	using consume_predicate_t = std::function<bool(Token &token)>;
	virtual optional<Token> consumeIf(consume_predicate_t predicate);
};

} // namespace shell
