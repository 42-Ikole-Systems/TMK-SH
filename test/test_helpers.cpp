#include "test_helpers.hpp"
#include <functional>
#include "catch.hpp"

void assertExpectedTokens(const string &line, vector<Token> &expected_tokens) {

	SECTION(line) {
		auto input = line + '\n';
		// Prepare the array of expected tokens
		vector<Token> expected;
		expected.reserve(expected_tokens.size());
		auto newline = Token(Token::Type::Newline, Newline());
		for (auto &token : expected_tokens) {
			expected.push_back(token);
		}
		expected.push_back(newline);

		auto chars = LineCharProvider(input);
		auto lexer = Lexer(chars);
		vector<Token> actual;
		while (true) {
			auto token = lexer.consume();
			if (!token.has_value()) {
				break;
			}
			actual.emplace_back(std::move(token.value()));
		}
		REQUIRE(expected == actual);
	}

}
