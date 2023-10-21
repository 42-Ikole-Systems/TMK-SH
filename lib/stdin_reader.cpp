#include "shell/stdin_reader.hpp"
#include <readline/readline.h>
#include <stdlib.h>
#include "shell/utility/types.hpp"
#include "shell/lexer/lexer.hpp"

namespace shell {

StdinReader::StdinReader(const string &prompt) : prompt(prompt), line(nullopt), lexer(nullptr) {
}

StdinReader::~StdinReader() {
}

optional<string> StdinReader::nextLine() {
	char *line = readline(getPrompt());
	if (line == nullptr) {
		return nullopt;
	}
	// todo: optimize string copying
	string result = string(line);
	free(line);
	return result;
}

char StdinReader::nextChar() {
	if (!line.has_value() || index >= line->length()) {
		line = nextLine();
		index = 0;
	}
	if (!line.has_value()) {
		return 0; // EOF
	}
	if (line->length() == 0) {
		return '\n';
	}
	return line->at(index++);
}

void StdinReader::setLexer(Lexer *lexer) {
	this->lexer = lexer;
}

const char *StdinReader::getPrompt() const {
	if (lexer == nullptr || lexer->getCurrentState() == Lexer::State::Empty) {
		return prompt.c_str();
	}
	return "> ";
}

} // namespace shell
