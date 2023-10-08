#include <stdbool.h>
#include <readline/readline.h>
#include "shell/stdin_reader.hpp"
#include "shell/parser.hpp"
#include "shell/util.hpp"
#include "shell/lexer/lexer.hpp"
#include "shell/lexer/line_char_provider.hpp"
#include <utility>
#include "shell/print.hpp"
#include "shell/executor/executor.hpp"

namespace shell {

// todo: parse PS1
static string prompt = "";

static void initialize() {
	if (prompt.empty()) {
		prompt = "tmksh $ ";
	}
}

static int run(int argc, const char **argv, char *const *envp) {
	initialize();

	using_history();
	StdinReader reader = StdinReader(prompt);
	// Lexer lexer = Lexer(reader);
	// Parser parser = Parser();
	while (true) {
		auto line = reader.nextLine();
		if (!line.has_value()) {
			break;
		}
		add_history(line.value().c_str());
		line.value().push_back('\n');
		auto chars = LineCharProvider(line.value());
		auto lexer = Lexer(chars);
		auto parser = Parser(lexer);
		Ast ast = parser.getNextCommand();
		ast.print();
		auto executor = Executor(envp);
		executor.execute(ast);
	}
	tprintf("\n");
	return 0;
}

} // namespace shell

int main(int argc, const char **argv, char *const *envp) {
	return shell::run(argc, argv, envp);
}
