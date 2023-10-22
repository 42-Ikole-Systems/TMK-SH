#include "shell/expansion/quote_removal.hpp"
#include "shell/utility/characters.hpp"
#include "shell/assert.hpp"

namespace shell {

QuoteRemover::State QuoteRemover::emptyState() {
	auto ch = input.peek();
	if (isBackslash(ch)) {
		input.consume();
		states.push(State::Empty);
		return State::BackSlash;
	}
	if (isDoubleQuote(ch)) {
		input.consume();
		return State::DoubleQuote;
	}
	if (isSingleQuote(ch)) {
		input.consume();
		return State::SingleQuote;
	}
	result.push_back(input.consume());
}

QuoteRemover::State QuoteRemover::backslashState() {
	D_ASSERT(!states.empty());
	result.push_back(input.consume());
	auto return_state = states.top();
	states.pop();
	return return_state;
}

QuoteRemover::State QuoteRemover::singleQuoteState() {
	auto ch = input.peek();
	while (!isSingleQuote(ch) && ch != EOF) {
		result.push_back(input.consume());
		ch = input.peek();
	}
	if (ch == EOF) {
		throw NotImplementedException("Deal with errors here");
	}
	D_ASSERT(isSingleQuote(ch));
	input.consume();
	return State::Empty;
}

QuoteRemover::State QuoteRemover::doubleQuoteState() {
	auto ch = input.peek();
	while (!isDoubleQuote(ch) && ch != EOF) {
		if (isBackslash(ch)) {
			states.push(State::DoubleQuote);
			return State::BackSlash;
		}
		result.push_back(input.consume());
		ch = input.peek();
	}
	if (ch == EOF) {
		throw NotImplementedException("Deal with errors here");
	}
	D_ASSERT(isDoubleQuote(ch));
	input.consume();
	return State::Empty;
}

std::function<QuoteRemover::State(QuoteRemover &)> QuoteRemover::getStateHandler() {
	static const std::function<QuoteRemover::State(QuoteRemover &)> handlers[] = {
	    [(int)QuoteRemover::State::Empty] = &emptyState,
	    [(int)QuoteRemover::State::BackSlash] = &backslashState,
	    [(int)QuoteRemover::State::SingleQuote] = &singleQuoteState,
	    [(int)QuoteRemover::State::DoubleQuote] = &doubleQuoteState,
	};
	D_ASSERT((size_t)state < (sizeof(handlers) / sizeof(handlers[0])));
	D_ASSERT(handlers[(int)state] != nullptr);
	return handlers[((int)state)];
}

string QuoteRemover::process() {
	D_ASSERT(!consumed);
	state = State::Empty;
	while (state != State::Done && input.peek() != EOF) {
		auto handler = getStateHandler();
		state = handler(*this);
	}
	consumed = true;
	return result;
}

} // namespace shell
