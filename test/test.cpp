#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "shell/stdin_reader.hpp"
#include "shell/parser.hpp"
#include "shell/utility/types.hpp"
#include "shell/lexer/lexer.hpp"
#include "shell/lexer/line_char_provider.hpp"
#include <utility>
#include "shell/print.hpp"
#include "shell/executor/executor.hpp"
#include "shell/environment.hpp"
#include "shell/global_environment.hpp"

using namespace shell;

extern char** environ;

TEST_CASE("basic shell", "[shell]") {
	GlobalEnvironment env;
	char** envp = environ;
	string line = "/bin/ls -la";

	line.push_back('\n');
	auto chars = LineCharProvider(line);
	auto lexer = Lexer(chars);
	auto parser = Parser(lexer);
	Ast ast = parser.parse();
	ast.print();
	auto executor = Executor(env);
	executor.execute(ast);
}
