#pragma once

#include "util.hpp"

namespace shell {

// https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html#tag_02_10_01
enum class TokenType {
	/* token / word */
	Token, // Word, Name, or Assignment depending on context
	Word,
	AssignmentWord, // Assignment (?)
	Name,

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

	/* not sure if these are operators? referenced: https://www.gnu.org/software/bash/manual/bash.html#Definitions as
	   control operators*/
	// SemicolonAnd, // ;&
	// DoubleSemicolonAnd, // ;;&
};

struct WordToken {
	string value;

	void print() const;
};

struct OperatorToken {
	OperatorToken();
	void print(TokenType type) const;
};

// std::variant?
struct Token {
	TokenType type;

	// todo: variant type
	// union {
	WordToken word_token;
	OperatorToken operator_token;
	// };

	void print() const;
};

} // namespace shell