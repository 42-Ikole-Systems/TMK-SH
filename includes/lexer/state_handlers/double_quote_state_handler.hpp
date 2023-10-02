#pragma once

#include "lexer.hpp"
#include "lexer/lexer_state.hpp"
#include "lexer/state_handler.hpp"

namespace shell {

struct DoubleQuoteStateHandler {
	static constexpr LexerState state = LexerState::DOUBLE_QUOTE;
	static LexerState nextState(char current_character);
	static constexpr StateHandler handler = StateHandler{
		.next_state = nextState
	};
};

} //namespace shell
