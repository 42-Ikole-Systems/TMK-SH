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
		tprintf("%\n", line.value());
		vector<Token> tokens = lexer.tokenize(line.value());
		for (const Token &token : tokens) {
			token.print();
		}
		Parser::Ast ast = parser.getNextCommand(tokens);
		ast.print();
	}
	tprintf("\n");
	return 0;
}

} // namespace shell

int main(int argc, char *argv[], char *envp[]) {
	return shell::run(argc, argv, envp);
}
