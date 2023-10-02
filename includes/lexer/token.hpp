#pragma once

#include "util.hpp"

namespace shell {

struct WordToken {
	string value;

	void print() const;
};

struct OperatorToken {
	enum class Type {
		Semicolon, Ampersand, Unknown
	};

	OperatorToken();
	OperatorToken(char ch);

	Type type;

	void print() const;
};

// std::variant?
struct Token {
	enum class Type { Word, Operator };

	Type type;
	// cannot have optional of a union type, or complex constructor inside of union type
	// union {
	WordToken word_token;
	OperatorToken operator_token;
	// };

	void print() const;
};

} // namespace shell