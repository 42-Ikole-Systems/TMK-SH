#pragma once

#include "shell/utility/types.hpp"
#include "shell/lexer/line_char_provider.hpp"

namespace shell {

class QuoteRemover {
private:
	enum class State { Done, Empty, BackSlash, DoubleQuote, SingleQuote };

public:
	QuoteRemover(const string &input) : input_string(input), input(input_string) {
	}

public:
	string process();

private:
	std::function<State(QuoteRemover &)> getStateHandler();
	State emptyState();
	State backslashState();
	State singleQuoteState();
	State doubleQuoteState();

	stack<State> states;

	State state;
	string input_string;
	LineCharProvider input;

	string result;
	bool consumed = false;
};

} // namespace shell
