#include "test_helpers.hpp"

void assertExpectedTokens(const string &line, vector<Token> &expected_tokens) {
	auto input = line + '\n';
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

	REQUIRE(expected_tokens.size() == actual.size());
	for (size_t i = 0; i < expected_tokens.size(); i++) {
		auto &a = expected_tokens[i];
		auto &b = actual[i];

		REQUIRE(a.equals(b));
	}
}
