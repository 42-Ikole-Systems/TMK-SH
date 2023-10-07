#pragma once

#include "grammar/rule.hpp"
#include <optional>
#include "ast.hpp"

namespace shell {

class ListRule {
public:
	static Rule make();
	static vector<Rule::Option> options();

private:
	static optional<Ast::Node> separatedList(const vector<Ast::Node> &nodes);
	static optional<Ast::Node> singleCommand(const vector<Ast::Node> &nodes);
};

} // namespace shell
