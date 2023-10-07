#pragma once

#include "grammar.hpp"
#include <optional>
#include "ast.hpp"

namespace shell {

class AndOr {
public:
	static GrammarRule make();
	static vector<GrammarRule::Option> options();

private:
	static optional<Ast::Node> handler(const vector<Ast::Node> &nodes);
};

} // namespace shell
