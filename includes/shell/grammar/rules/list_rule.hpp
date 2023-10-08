#pragma once

#include "shell/grammar/rule.hpp"
#include <optional>
#include "shell/ast.hpp"

namespace shell {

class ListRule {
public:
	static Rule make();
	static vector<Rule::Option> options();

private:
	static optional<Ast::Node> separatedList(vector<Ast::Node> &nodes);
	static optional<Ast::Node> singleCommand(vector<Ast::Node> &nodes);
};

} // namespace shell
