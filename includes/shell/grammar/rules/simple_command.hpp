#pragma once

#include "shell/grammar/rule.hpp"
#include <optional>
#include "shell/ast.hpp"

namespace shell {

class SimpleCommand {
public:
	static Rule make();
	static vector<Rule::Option> options();

private:
	static optional<Ast::Node> singleCommand(vector<Ast::Node> &args);
};

} // namespace shell
