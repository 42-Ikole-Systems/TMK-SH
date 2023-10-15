#include "shell/lexer/reader_char_provider.hpp"
#include "shell/assert.hpp"

namespace shell {

ReaderCharProvider::ReaderCharProvider(Reader &reader) : reader(reader), index(0), eof(false) {
}

char ReaderCharProvider::peek() {
	if (!ensureChar()) {
		return EOF;
	}
	return line[index];
}

char ReaderCharProvider::consume() {
	if (!ensureChar()) {
		return EOF;
	}
	return line[index++];
}

// char at index is removed, index doesn't change
void ReaderCharProvider::remove() {
	if (!ensureChar()) {
		return;
	}
	line.erase(line.begin() + index);
}

void ReaderCharProvider::unconsume() {
	D_ASSERT(index > 0);
	index--;
}

bool ReaderCharProvider::ensureChar() {
	if (eof) {
		return false;
	}
	if (index < line.size()) {
		return true;
	}
	auto next = reader.nextLine();
	if (!next.has_value()) {
		eof = true;
		return false;
	}
	line.append(next.value());
	// also add newline to end of stream
	line.push_back('\n');
	return true;
}

string ReaderCharProvider::takeLine() {
	string result = std::move(line);
	reset();
	// remove new line from end
	D_ASSERT(result.back() == '\n');
	result.pop_back();
	return result;
}

// just use RAII .. i.e. destruct
void ReaderCharProvider::reset() {
	eof = false;
	line.clear();
	index = 0;
}

bool ReaderCharProvider::isEof() const {
	return eof;
}

} // namespace shell
