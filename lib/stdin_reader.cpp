#include "shell/stdin_reader.hpp"
#include <readline/readline.h>
#include <stdlib.h>
#include "shell/utility/types.hpp"

namespace shell {

StdinReader::StdinReader(const string &prompt) : prompt(prompt), line(nullopt) {
}

StdinReader::~StdinReader() {
}

optional<string> StdinReader::nextLine() {
	char *line = readline(prompt.c_str());
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

} // namespace shell
