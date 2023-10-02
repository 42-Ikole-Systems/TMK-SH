#include "lexer.hpp"
#include <stdexcept>
#include "util.hpp"

namespace shell {

const string &LineReaderState::getLine() {
	if (current_line == std::nullopt || current_index >= current_line->size()) {
		current_line = reader.nextLine();
		current_index = 0;
		// TODO: handle returning nullopt
	}
	return *current_line;
}

char LineReaderState::peek() {
	auto &line = getLine();
	return line[current_index];
}

char LineReaderState::consume() {
	auto &line = getLine();
	auto value = peek();
	current_index++;
}

Lexer::Lexer(unique_ptr<Reader> reader_p) : reader(std::move(reader_p)), line_reader_state(*reader) {}

void Lexer::updateState() {
	auto current = line_reader_state.peek();
}

Lexer::Token Lexer::getNextToken() {
	do {
		updateState();
	} while (state != State::END_OF_TOKEN);
}

}; //namespace shell
