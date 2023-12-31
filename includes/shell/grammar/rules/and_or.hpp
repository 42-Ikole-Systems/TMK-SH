#pragma once

#include "shell/grammar/rule.hpp"
#include <optional>
#include "shell/ast.hpp"

namespace shell {

class AndOr {
public:
	static Rule make();
	static vector<Rule::Option> options();

private:
	static optional<Ast::Node> handler(vector<Ast::Node> &args);
};

} // namespace shell
