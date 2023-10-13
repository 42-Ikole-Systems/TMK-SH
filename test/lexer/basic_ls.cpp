#include "test_helpers.hpp"

using namespace shell;

TEST_CASE("lexer parameterized test", "[lexer][parameterized]") {
	auto input = GENERATE(
		make_pair("/bin/ls", vector { Token(Token::Type::Word, WordToken{"/bin/ls"})}),
		make_pair("<<-", vector { Token(Token::Type::DoubleLessDash, OperatorToken{})}),
		make_pair("<-", vector { Token(Token::Type::Less, OperatorToken{}), Token({Token::Type::Word, WordToken{"-"}})}),
		make_pair("<\\\n<", vector { Token(Token::Type::DoubleLess, OperatorToken{})}),
		make_pair("ex\\\nample", vector { Token(Token::Type::Word, WordToken{"example"})}),
		make_pair("ex\\\\", vector { Token(Token::Type::Word, WordToken{"ex\\\\"})}),
		make_pair("\\<<", vector { Token(Token::Type::Word, WordToken{"\\<"}), Token(Token::Type::Less, OperatorToken{}) }),
		make_pair("'\\<<'\"abcde\\\n\"", vector { Token(Token::Type::Word, WordToken{"'\\<<'\"abcde\""}) }),
		make_pair("< <\\\n<", vector { Token(Token::Type::Less, OperatorToken{}), Token(Token::Type::DoubleLess, OperatorToken{}) })
	);

	assertExpectedTokens(input.first, input.second);
}


TEST_CASE("ls -la", "[lexer]") {
	string line = "/bin/ls -la";
	vector<Token> expected = {
		Token(Token::Type::Word, WordToken{"/bin/ls"}),
		Token(Token::Type::Word, WordToken{"-la"})
	};
	assertExpectedTokens(line, expected);
}

TEST_CASE("greater token", "[lexer]") {
	vector<Token> expected = {
		Token(Token::Type::Word, WordToken{"/bin/ls"}),
		Token(Token::Type::Great, OperatorToken()),
		Token(Token::Type::Word, WordToken{"bla"})
	};
	assertExpectedTokens("/bin/ls > bla", expected);
	assertExpectedTokens("/bin/ls >bla", expected);
	assertExpectedTokens("/bin/ls>bla", expected);
}

TEST_CASE("double greater token", "[lexer]") {
	vector<Token> expected = {
		Token(Token::Type::Word, WordToken{"/bin/ls"}),
		Token(Token::Type::DoubleGreat, OperatorToken()),
		Token(Token::Type::Word, WordToken{"bla"})
	};
	assertExpectedTokens("/bin/ls >> bla", expected);
	assertExpectedTokens("/bin/ls >>bla", expected);
	assertExpectedTokens("/bin/ls>>bla", expected);
}

TEST_CASE("double less token", "[lexer]") {
	vector<Token> expected = {
		Token(Token::Type::Word, WordToken{"/bin/ls"}),
		Token(Token::Type::DoubleLess, OperatorToken()),
		Token(Token::Type::Word, WordToken{"bla"})
	};
	assertExpectedTokens("/bin/ls << bla", expected);
	assertExpectedTokens("/bin/ls <<bla", expected);
	assertExpectedTokens("/bin/ls<<bla", expected);
}

TEST_CASE("double less dash", "[lexer]") {
	vector<Token> expected = {
		Token(Token::Type::Word, WordToken{"/bin/ls"}),
		Token(Token::Type::DoubleLessDash, OperatorToken()),
		Token(Token::Type::Word, WordToken{"bla"})
	};
	assertExpectedTokens("/bin/ls <<- bla", expected);
	assertExpectedTokens("/bin/ls <<-bla", expected);
	assertExpectedTokens("/bin/ls<<-bla", expected);

	// Check that the operator token still gets properly delimited at the end
	expected.pop_back();
	assertExpectedTokens("/bin/ls <<- ", expected);
	assertExpectedTokens("/bin/ls <<-", expected);
	assertExpectedTokens("/bin/ls<<-", expected);
}

TEST_CASE("less token", "[lexer]") {
	vector<Token> expected = {
		Token(Token::Type::Word, WordToken{"/bin/ls"}),
		Token(Token::Type::Less, OperatorToken()),
		Token(Token::Type::Word, WordToken{"bla"})
	};
	assertExpectedTokens("/bin/ls < bla", expected);
	assertExpectedTokens("/bin/ls <bla", expected);
	assertExpectedTokens("/bin/ls<bla", expected);
}
