#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "shell/stdin_reader.hpp"
#include "shell/parser.hpp"
#include "shell/util.hpp"
#include "shell/lexer/lexer.hpp"
#include "shell/lexer/line_char_provider.hpp"
#include <utility>
#include "shell/print.hpp"
#include "shell/executor/executor.hpp"

using namespace shell;

extern char** environ;

TEST_CASE("lexer test", "[shell]") {
	string line = "/bin/ls -la";

	line.push_back('\n');
	auto chars = LineCharProvider(line);
	auto lexer = Lexer(chars);
	vector<Token> actual;
	while (true) {
		auto token = lexer.consume();
		if (!token.has_value()) {
			break;
		}
		actual.emplace_back(std::move(token.value()));
	}

	vector<Token> expected = {
		Token(Token::Type::Word, WordToken{"/bin/ls"}),
		Token(Token::Type::Word, WordToken{"-la"}),
		Token(Token::Type::Newline, Newline())
	};
	REQUIRE(expected.size() == actual.size());
	for (size_t i = 0; i < expected.size(); i++) {
		auto &a = expected[i];
		auto &b = actual[i];

		REQUIRE(a.equals(b));
	}
}

TEST_CASE("basic shell", "[shell]") {
	char** envp = environ;
	string line = "/bin/ls -la";

	line.push_back('\n');
	auto chars = LineCharProvider(line);
	auto lexer = Lexer(chars);
	auto parser = Parser(lexer);
	Ast ast = parser.parse();
	ast.print();
	auto executor = Executor(envp);
	executor.execute(ast);
}
