#pragma once

#include "shell/util.hpp"

namespace shell {

/* specific token values */
struct WordToken {
	string value;
	string toString() const;
};

struct OperatorToken {};

struct IoNumber {
	string value; // numeric string
	string toString() const;
};

struct Newline {};

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
		AndIf,            // &&
		OrIf,             // ||
		DoubleSemicolon,  // ;;
		Less,             // <
		DoubleLess,       // <<
		Great,            // >
		DoubleGreat,      // >>
		LessAnd,          // <&
		GreatAnd,         // >&
		LessGreat,        // <>
		DoubleLessDash,   // <<-
		Clobber,          // >|

		/* not sure if these are operators? referenced: https://www.gnu.org/software/bash/manual/bash.html#Definitions
		as control operators*/
		// SemicolonAnd, // ;&
		// DoubleSemicolonAnd, // ;;&
	};

private:
	Type type;
	Variant variant;

	static const vector<pair<string, Type>> operator_types;

public:
	Token() = default;                   // remove
	Token(const Token &other) = default; // remove and optimize code to not copy but move instead as much as possible

	Token(Token &&other) = default;
	Token(Type type, Variant &&variant);
	Token &operator=(Token &&other) = default;

	Type getType() const;
	void print() const;
	string toString() const;

	bool equals(const Token &other) const;

	template <typename T>
	T &get() {
		return std::get<T>(variant);
	}

	template <typename T>
	const T &get() const {
		return std::get<T>(variant);
	}

	static int prefixOperatorMatches(const string &s);
	static optional<Type> exactOperatorType(const string &s);

private:
	static const string &getOperatorString(Token::Type type);
	static bool isOperator(Token::Type type);
	static bool isWord(Token::Type type);
	static bool isIoNumber(Token::Type type);
	static bool isNewline(Token::Type type);
};

bool operator==(const Token& lhs, const Token& rhs);
std::ostream& operator<<(std::ostream& lhs, const Token& rhs);

} // namespace shell