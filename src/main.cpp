#include <stdbool.h>
#include <readline/readline.h>
#include "stdin_reader.hpp"
#include "parser.hpp"
#include "util.hpp"
#include "lexer/lexer.hpp"
#include <utility>
#include "print.hpp"

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

	StdinReader reader = StdinReader(prompt);
	Lexer lexer = Lexer(reader);
	Parser parser = Parser();
	while (true) {
		auto line = reader.nextLine();
		if (!line.has_value()) {
			break;
		}
		tprintf("%s\n", line.value());
		vector<Token> tokens = lexer.lexTokens(line.value());
		Parser::AST ast = parser.getNextCommand(tokens);
	}
	return 0;
}

} // namespace shell

int main(int argc, char *argv[], char *envp[]) {
	return shell::run(argc, argv, envp);
}
