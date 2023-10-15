#pragma once

#include "shell/interfaces/reader.hpp"
#include "shell/util.hpp"

namespace shell {

class Lexer;

class StdinReader : public Reader {
private:
	string prompt;
	optional<string> line;
	size_t index;
	Lexer *lexer; // use lexer state to determine prompt

public:
	StdinReader(const string &prompt);
	virtual ~StdinReader();

	optional<string> nextLine() override;
	char nextChar() override;
	void setLexer(Lexer *lexer);

private:
	const char *getPrompt() const;
};

} // namespace shell
