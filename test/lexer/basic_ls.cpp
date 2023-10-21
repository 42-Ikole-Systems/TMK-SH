#include "test_helpers.hpp"

using namespace shell;

TEST_CASE("lexer parameterized test", "[lexer][parameterized]") {
	auto input = GENERATE(
		make_pair("/bin/ls", vector { Token(Token::Type::Token, WordToken{"/bin/ls"})}),
		make_pair("echo \\`", vector { Token(Token::Type::Token, WordToken{"echo"}), Token(Token::Type::Token, WordToken{"\\`"})}),
		make_pair("<<-", vector { Token(Token::Type::DoubleLessDash, OperatorToken{})}),
		make_pair("<-", vector { Token(Token::Type::Less, OperatorToken{}), Token({Token::Type::Token, WordToken{"-"}})}),
		make_pair("<\\\n<", vector { Token(Token::Type::DoubleLess, OperatorToken{})}),
		make_pair("ex\\\nample", vector { Token(Token::Type::Token, WordToken{"example"})}),
		make_pair("ex\\\\", vector { Token(Token::Type::Token, WordToken{"ex\\\\"})}),
		make_pair("\\<<", vector { Token(Token::Type::Token, WordToken{"\\<"}), Token(Token::Type::Less, OperatorToken{}) }),
		make_pair("'\\<<'\"abcde\\\n\"", vector { Token(Token::Type::Token, WordToken{"'\\<<'\"abcde\\\n\""}) }), // everything should be present in the resulting string
		make_pair("< <\\\n<", vector { Token(Token::Type::Less, OperatorToken{}), Token(Token::Type::DoubleLess, OperatorToken{}) }),

		make_pair("`echo 1234`", vector { Token( Token::Type::Token, WordToken{ "`echo 1234`"} )}),
		make_pair(
			"   `\\`echo 1234 \\` $() ${} $ $(())`    ",
			vector { Token(Token::Type::Token, WordToken{"`\\`echo 1234 \\` $() ${} $ $(())`"})}
		),

		make_pair(
			"\"$(   ')' \") $( \"abcde\" )\" \\)  )\"",
			vector { Token(Token::Type::Token, WordToken{"\"$(   ')' \") $( \"abcde\" )\" \\)  )\""} )}
		),

		make_pair(
			"$((5 + 9))",
			vector { Token(Token::Type::Token, WordToken{"$((5 + 9))"})}
		),

		make_pair("$(( 5 + 9 )) $(( echo \"${HOME}\" ) | cat  )",
			vector {
				Token( Token::Type::Token, WordToken{"$(( 5 + 9 ))"}),
				Token( Token::Type::Token, WordToken{"$(( echo \"${HOME}\" ) | cat  )"})
			}
		),

		make_pair("5> 12934712834< 1a2> a;", vector {
			Token( Token::Type::IoNumber, IoNumber{"5"} ), Token( Token::Type::Great, OperatorToken{} ),
			Token( Token::Type::IoNumber, IoNumber{"12934712834"} ), Token( Token::Type::Less, OperatorToken{} ),
			Token( Token::Type::Token, WordToken{"1a2"} ), Token( Token::Type::Great, OperatorToken{} ),
			Token( Token::Type::Token, WordToken{"a"} ), Token( Token::Type::Semicolon, OperatorToken{} )
		})
	);

	assertExpectedTokens(input.first, input.second);
}


TEST_CASE("ls -la", "[lexer]") {
	string line = "/bin/ls -la";
	vector<Token> expected = {
		Token(Token::Type::Token, WordToken{"/bin/ls"}),
		Token(Token::Type::Token, WordToken{"-la"})
	};
	assertExpectedTokens(line, expected);
}

TEST_CASE("greater token", "[lexer]") {
	vector<Token> expected = {
		Token(Token::Type::Token, WordToken{"/bin/ls"}),
		Token(Token::Type::Great, OperatorToken()),
		Token(Token::Type::Token, WordToken{"bla"})
	};
	assertExpectedTokens("/bin/ls > bla", expected);
	assertExpectedTokens("/bin/ls >bla", expected);
	assertExpectedTokens("/bin/ls>bla", expected);
}

TEST_CASE("double greater token", "[lexer]") {
	vector<Token> expected = {
		Token(Token::Type::Token, WordToken{"/bin/ls"}),
		Token(Token::Type::DoubleGreat, OperatorToken()),
		Token(Token::Type::Token, WordToken{"bla"})
	};
	assertExpectedTokens("/bin/ls >> bla", expected);
	assertExpectedTokens("/bin/ls >>bla", expected);
	assertExpectedTokens("/bin/ls>>bla", expected);
}

TEST_CASE("double less token", "[lexer]") {
	vector<Token> expected = {
		Token(Token::Type::Token, WordToken{"/bin/ls"}),
		Token(Token::Type::DoubleLess, OperatorToken()),
		Token(Token::Type::Token, WordToken{"bla"})
	};
	assertExpectedTokens("/bin/ls << bla", expected);
	assertExpectedTokens("/bin/ls <<bla", expected);
	assertExpectedTokens("/bin/ls<<bla", expected);
}

TEST_CASE("double less dash", "[lexer]") {
	vector<Token> expected = {
		Token(Token::Type::Token, WordToken{"/bin/ls"}),
		Token(Token::Type::DoubleLessDash, OperatorToken()),
		Token(Token::Type::Token, WordToken{"bla"})
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
		Token(Token::Type::Token, WordToken{"/bin/ls"}),
		Token(Token::Type::Less, OperatorToken()),
		Token(Token::Type::Token, WordToken{"bla"})
	};
	assertExpectedTokens("/bin/ls < bla", expected);
	assertExpectedTokens("/bin/ls <bla", expected);
	assertExpectedTokens("/bin/ls<bla", expected);
}
