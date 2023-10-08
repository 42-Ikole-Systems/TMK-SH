#include "test_helpers.hpp"

using namespace shell;

TEST_CASE("ls -la", "[lexer]") {
	string line = "/bin/ls -la";
	vector<Token> expected = {
		Token(Token::Type::Word, WordToken{"/bin/ls"}),
		Token(Token::Type::Word, WordToken{"-la"}),
		Token(Token::Type::Newline, Newline())
	};
	assertExpectedTokens(line, expected);
}

TEST_CASE("greater token", "[lexer]") {
	vector<Token> expected = {
		Token(Token::Type::Word, WordToken{"/bin/ls"}),
		Token(Token::Type::Great, OperatorToken()),
		Token(Token::Type::Word, WordToken{"bla"}),
		Token(Token::Type::Newline, Newline())
	};
	assertExpectedTokens("/bin/ls > bla", expected);
	assertExpectedTokens("/bin/ls >bla", expected);
}

TEST_CASE("less token", "[lexer]") {
	vector<Token> expected = {
		Token(Token::Type::Word, WordToken{"/bin/ls"}),
		Token(Token::Type::Less, OperatorToken()),
		Token(Token::Type::Word, WordToken{"bla"}),
		Token(Token::Type::Newline, Newline())
	};
	assertExpectedTokens("/bin/ls < bla", expected);
	assertExpectedTokens("/bin/ls <bla", expected);
}
