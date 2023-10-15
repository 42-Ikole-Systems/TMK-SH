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
#include "shell/lexer/reader_char_provider.hpp"

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
	auto provider = ReaderCharProvider(reader);
	while (true) {
		auto lexer = Lexer(provider);
		auto parser = Parser(lexer);
		Ast ast = parser.parse();
		ast.print();
		if (provider.isEof()) {
			break;
		}
		string line = provider.takeLine();
		if (!line.empty()) {
			add_history(line.c_str());
		}
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
