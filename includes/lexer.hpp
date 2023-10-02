#pragma once

#include "reader.hpp"
#include "util.hpp"
#include "lexer/line_reader_state.hpp"

namespace shell {

class Lexer {
private:
	optional<string> current_line;
	unique_ptr<Reader> reader;
	LineReaderState line_reader_state;
	enum class State : uint8_t {
		END_OF_TOKEN,
		DEFAULT
	};
	State state = State::DEFAULT;

public:
	Lexer(unique_ptr<Reader> reader);
	Lexer(Lexer &&other) = default;

	struct Token {
		enum class TokenType : uint8_t {
			WORD,
			OPERATOR
		};
		using Type = TokenType;
		Token(Type type) : type(type) {}
		Type type;
	};

	/**
	 * @brief Get the Next Token object
	 * May read
	 * @return Token
	 */
	Token getNextToken();
private:
	void updateState();
};

class WordToken : public Lexer::Token {
	using base = Lexer::Token;
	using Type = base::Type;
	WordToken() : base(Type::WORD) {}
};

} // namespace shell
