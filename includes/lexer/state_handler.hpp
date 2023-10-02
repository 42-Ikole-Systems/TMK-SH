#pragma once

#include "util.hpp"
#include "reader.hpp"
#include "lexer/lexer_state.hpp"

namespace shell {

struct StateHandler;

typedef LexerState (*next_state_t)(char current_character);

struct StateHandler {
	next_state_t next_state;
};

} //namespace shell
