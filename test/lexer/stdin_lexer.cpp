#include <catch.hpp>
#include "test_helpers.hpp"
#include "shell/lexer/reader_char_provider.hpp"
#include "shell/logger.hpp"
#include "readline/readline.h"

// skip, only run manually for testing stdin prompts
TEST_CASE("lexer stdin test", "[.][manual][lexer]") {
	using_history();
	StdinReader reader("> ");
	while (true) {
		auto charProvider = ReaderCharProvider(reader);
		Lexer lexer(charProvider);
		while (true) {
			auto token = lexer.consume();
			if (!token.has_value() || token->getType() == Token::Type::Newline) {
				break;
			}
		}
		if (charProvider.isEof()) {
			break;
		}
		string line = charProvider.takeLine();
		if (!line.empty()) {
			add_history(line.c_str());
		}
	}
}
