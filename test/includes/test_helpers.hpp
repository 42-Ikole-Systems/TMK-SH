#pragma once

#include "catch.hpp"
#include "shell/stdin_reader.hpp"
#include "shell/parser.hpp"
#include "shell/utility/types.hpp"
#include "shell/lexer/lexer.hpp"
#include "shell/lexer/line_char_provider.hpp"
#include <utility>
#include "shell/print.hpp"
#include "shell/executor/executor.hpp"

using namespace shell;

void assertExpectedTokens(const string &line, vector<Token> &expected_tokens);
