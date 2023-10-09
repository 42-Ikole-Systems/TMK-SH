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
};

} // namespace shell