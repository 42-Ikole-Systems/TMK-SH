#include "lexer/state_handlers/double_quote_state_handler.hpp"

namespace shell {

LexerState DoubleQuoteStateHandler::nextState(char current_character) {
	if (current_character == '"') {
		return LexerState::NEUTRAL;
	}
}

} //namespace shell
