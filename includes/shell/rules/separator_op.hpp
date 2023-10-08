#pragma once

#include "shell/grammar.hpp"
#include <optional>
#include "shell/ast.hpp"

namespace shell {

class SeparatorOp {
public:
	static GrammarRule make();
	static vector<GrammarRule::Option> options();

private:
	static optional<Ast::Node> handler(const vector<Ast::Node> &nodes);
};

} // namespace shell
