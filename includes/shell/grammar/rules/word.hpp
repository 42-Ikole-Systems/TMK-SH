#pragma once

#include "shell/grammar/rule.hpp"
#include <optional>
#include "shell/ast.hpp"

namespace shell {

class Word {
public:
	static Rule make();
	static vector<Rule::Option> options();

public:
	static optional<Ast::Node> handler(TokenProvider &tokens);
};

} // namespace shell
