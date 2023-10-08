#pragma once

#include "shell/grammar/rule.hpp"
#include "shell/interfaces/provider.hpp"
#include <optional>
#include "shell/ast.hpp"
#include "shell/lexer/token_provider.hpp"

namespace shell {

class SeparatorOp {
public:
	static Rule make();
	static vector<Rule::Option> options();
};

} // namespace shell
