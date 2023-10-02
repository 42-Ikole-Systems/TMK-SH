#pragma once

#include "util.hpp"
#include "reader.hpp"

namespace shell {

enum class LexerState : uint8_t {
	DOUBLE_QUOTE
};

} //namespace shell
