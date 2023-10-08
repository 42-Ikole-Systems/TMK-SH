#pragma once

#include "util.hpp"

namespace shell {

/* specific token values */
struct WordToken {
	string value;
	void print() const;
};

struct OperatorToken {};

struct IoNumber {
	string value; // numeric string
	void print() const;
};

struct Newline {
	void print() const;
};

struct Token {
public:
	using Variant = std::variant<WordToken, OperatorToken, IoNumber, Newline>;

	// https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html#tag_02_10_01
	enum class Type {
		/* token / word */
		// Token, // Word, Name, or Assignment depending on context
		Word,
		// AssignmentWord, // Assignment (?)
		// Name,

		IoNumber, // e.g. 5>
		Newline,

		/* operators */
		Semicolon,        // ;
		And,              // &
		Pipe,             // | (pipe?)
		PipeAnd,          // |&
		ParenthesisOpen,  // (
		ParenthesisClose, // )

		AndIf,           // &&
		OrIf,            // ||
		DoubleSemicolon, // ;;
		DoubleLess,      // <<
		DoubleGreat,     // >>
		LessAnd,         // <&
		GreatAnd,        // >&
		LessGreat,       // <>
		DoubleLessDash,  // <<-
		Clobber,         // >|

		/* not sure if these are operators? referenced: https://www.gnu.org/software/bash/manual/bash.html#Definitions
		as control operators*/
		// SemicolonAnd, // ;&
		// DoubleSemicolonAnd, // ;;&
	};

private:
	Type type;
	Variant variant;

public:
	Token() = default;                   // remove
	Token(const Token &other) = default; // remove and optimize code to not copy but move instead as much as possible

	Token(Token &&other) = default;
	Token(Type type, Variant &&variant);
	Token &operator=(Token &&other) = default;

	Type getType() const;
	void print() const;

	template <typename T>
	T &get() {
		return std::get<T>(variant);
	}

	template <typename T>
	const T &get() const {
		return std::get<T>(variant);
	}
};

} // namespace shell