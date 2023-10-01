#include <stdbool.h>
#include <readline/readline.h>
#include "stdin_reader.hpp"
#include "parser.hpp"
#include "util.hpp"
#include <utility>

namespace shell {

// todo: parse PS1
static string prompt = "";

static void initialize() {
	if (prompt.empty()) {
		prompt = "tmksh $ ";
	}
}

static int run(int argc, char *argv[], char *env[]) {
	initialize();

	auto reader = make_unique<StdinReader>(prompt);
	Lexer lexer = Lexer(std::move(reader));
	Parser parser = Parser(std::move(lexer));
	while (true) {
		Parser::AST ast = parser.getNextCommand();
	}
	return 0;
}

} // namespace shell

int main(int argc, char *argv[], char *envp[]) {
	return shell::run(argc, argv, envp);
}
