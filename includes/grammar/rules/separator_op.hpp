#pragma once

#include "grammar/rule.hpp"
#include "interfaces/provider.hpp"
#include <optional>
#include "ast.hpp"
#include "lexer/token_provider.hpp"

namespace shell {

class SeparatorOp {
public:
	static Rule make();
	static vector<Rule::Option> options();
};

} // namespace shell
