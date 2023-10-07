#pragma once

#include "grammar/rule.hpp"
#include <optional>
#include "ast.hpp"

namespace shell {

template <class TOKEN_TYPE>
class Empty {
public:
	static Rule make() {
		return Rule {
			handler;
		};
	}
	static vector<Rule::Option> options(Producer<Token> &tokens) {
		return {};
	}

private:
	static optional<Ast::Node> handler(const vector<Ast::Node> &nodes) {
		return Node::Empty;
	}
};

} // namespace shell
