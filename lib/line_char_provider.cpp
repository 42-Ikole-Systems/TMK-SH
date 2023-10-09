#include "shell/lexer/line_char_provider.hpp"
#include "shell/utility/types.hpp"

namespace shell {

LineCharProvider::LineCharProvider(const string &line) : line(line), index(0) {
}

char LineCharProvider::peek() {
	if (index >= line.length()) {
		return EOF;
	}
	return line[index];
}

char LineCharProvider::consume() {
	if (index >= line.length()) {
		return EOF;
	}
	return line[index++];
}

} // namespace shell