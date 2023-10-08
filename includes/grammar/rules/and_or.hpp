#pragma once

#include "grammar/rule.hpp"
#include <optional>
#include "ast.hpp"

namespace shell {

class AndOr {
public:
	static Rule make();
	static vector<Rule::Option> options();

private:
	static optional<Ast::Node> handler(TokenProvider &tokens);
};

} // namespace shell
