#include "shell/stdin_reader.hpp"
#include "shell/parser.hpp"
#include "shell/utility/types.hpp"
#include "shell/lexer/lexer.hpp"
#include "shell/lexer/line_char_provider.hpp"
#include "shell/print.hpp"
#include "shell/executor/executor.hpp"
#include "shell/error/error.hpp"
#include "shell/shell_environment.hpp"
#include "shell/lexer/reader_char_provider.hpp"
#include "shell/logger.hpp"

#include <readline/readline.h>
#include <readline/history.h>

#include <stdbool.h>
#include <utility>

namespace shell {

// todo: parse PS1
static string prompt = "";
static constexpr const char *SHELL = "TMK-SH";

static void initialize() {
	if (prompt.empty()) {
		prompt = "tmksh $ ";
	}
}

static int run(int argc, const char **argv, char *const *envp) {
	initialize();

	using_history();
	StdinReader reader = StdinReader(prompt);
	while (true) {
		try {
			auto provider = ReaderCharProvider(reader);
			auto lexer = Lexer(provider);
			reader.setLexer(&lexer);
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
			ShellEnvironment environment;
			auto executor = Executor(environment);
			executor.execute(ast);
		} catch (const SyntaxErrorException e) {
			LOG_ERROR("%: syntax error: %\n", SHELL, e.what());
		} catch (const RecoverableException e) {
			LOG_ERROR("%: recoverable error: %\n", SHELL, e.what());
		}
	}
	tprintf("\n");
	return 0;
}

} // namespace shell

int main(int argc, const char **argv, char *const *envp) {
	return shell::run(argc, argv, envp);
}
