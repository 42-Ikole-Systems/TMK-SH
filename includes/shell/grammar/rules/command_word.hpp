#pragma once

#include "shell/grammar/rule.hpp"
#include <optional>
#include "shell/ast.hpp"

namespace shell {

class CommandWord {
public:
	static Rule make();
	static vector<Rule::Option> options();
};

} // namespace shell
