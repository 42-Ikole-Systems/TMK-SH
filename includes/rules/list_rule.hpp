#pragma once

#include "grammar.hpp"
#include <optional>
#include "ast.hpp"

namespace shell {

class ListRule {
public:
	static GrammarRule make();
	static vector<GrammarRule::Option> options();

private:
	static optional<Ast::Node> separatedList(const vector<Ast::Node> &nodes);
	static optional<Ast::Node> singleCommand(const vector<Ast::Node> &nodes);
};

} // namespace shell
