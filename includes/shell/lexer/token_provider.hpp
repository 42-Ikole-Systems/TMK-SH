#pragma once

#include "shell/lexer/token.hpp"
#include "shell/util.hpp"

namespace shell {

class TokenProvider {
public:
	virtual ~TokenProvider() {
	}

	virtual optional<Token> peek() = 0;
	virtual optional<Token> consume() = 0;
	virtual optional<Token> consumeIf(std::function<bool(Token &token)> predicate) {
		auto token = peek();
		if (!token.has_value()) {
			return nullopt;
		}
		if (predicate(token.value())) {
			consume();
			return token;
		}
		return nullopt;
	}
};

} // namespace shell
