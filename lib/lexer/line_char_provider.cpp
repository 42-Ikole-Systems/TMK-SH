#include "shell/lexer/line_char_provider.hpp"
#include "shell/util.hpp"

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

void  LineCharProvider::remove() {
	if (index >= line.length()) {
		return;
	}
	index++;
}

void  LineCharProvider::unconsume() {
	if (index == 0) {
		return;
	}
	index--;
}

} // namespace shell