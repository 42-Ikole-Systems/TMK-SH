#pragma once

#include "util.hpp"
#include "reader.hpp"

namespace shell {

struct LineReaderState {
public:
	LineReaderState(Reader &reader) : reader(reader) {}
private:
	optional<string> current_line;
	size_t current_index;
	Reader &reader;
public:
	char peek();
	char consume();
private:
	const string &getLine();
};

} //namespace shell
