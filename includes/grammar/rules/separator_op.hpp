#pragma once

#include "grammar/rule.hpp"
#include "interfaces/provider.hpp"
#include <optional>
#include "ast.hpp"

namespace shell {

class SeparatorOp {
public:
	static Rule make();
	static vector<Rule::Option> options();

private:
	static optional<Ast::Node> semicolon(TokenProvider &tokens);
	static optional<Ast::Node> ampersand(TokenProvider &tokens);
};

} // namespace shell
