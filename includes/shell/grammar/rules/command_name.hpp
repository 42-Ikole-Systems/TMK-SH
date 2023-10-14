#pragma once

#include "shell/grammar/rule.hpp"
#include <optional>
#include "shell/ast.hpp"

namespace shell {

class CommandName {
public:
	static Rule make();
	static vector<Rule::Option> options();

private:
	static optional<Ast::Node> handler(TokenProvider &tokens);
};

} // namespace shell
