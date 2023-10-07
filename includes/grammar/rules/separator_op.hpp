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
	static optional<Ast::Node> semicolon(Provider<Token> &tokens);
	static optional<Ast::Node> ampersand(Provider<Token> &tokens);
};

} // namespace shell
