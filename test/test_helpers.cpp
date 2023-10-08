#include "test_helpers.hpp"
#include <functional>

void assertExpectedTokens(const string &line, vector<Token> &expected_tokens) {
	auto input = line + '\n';

	// Prepare the array of expected tokens
	vector<std::reference_wrapper<Token>> expected;
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

	REQUIRE(expected.size() == actual.size());
	for (size_t i = 0; i < expected.size(); i++) {
		auto &a = expected[i].get();
		auto &b = actual[i];

		REQUIRE(a.equals(b));
	}
}
