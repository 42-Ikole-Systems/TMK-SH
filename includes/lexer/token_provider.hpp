#pragma once

#include "token.hpp"
#include "util.hpp"

namespace shell {

class TokenProvider {
public:
	virtual ~TokenProvider() {
	}

	virtual optional<Token> peek() = 0;
	virtual optional<Token> consume() = 0;
};

} // namespace shell