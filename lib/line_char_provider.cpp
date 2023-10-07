#include "lexer/line_char_provider.hpp"
#include "util.hpp"

namespace shell {

LineCharProvider::LineCharProvider(const string &line) : line(line), index(0) {
}

char LineCharProvider::peek(size_t n) {
	if (n > line.length() || index + n >= line.length()) {
		return EOF;
	}
	return line[index + n];
}

char LineCharProvider::consume(size_t n) {
	if (n > line.length() || index + n >= line.length()) {
		return EOF;
	}
	index += n;
	return line[index++];
}

} // namespace shell