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
