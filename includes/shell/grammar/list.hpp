#pragma once

#include "shell/ast.hpp"
#include "shell/grammar/rule.hpp"

namespace shell {

struct ListConstructor {
	ListConstructor() = delete;

public:
	static Rule::grammar_func_t ListCreator(std::function<Ast::Node(Ast::Node)> func);
	static Rule::grammar_func_t ListAppender(std::function<Ast::Node(Ast::Node)> func);

public:
	static optional<Ast::Node> CreateList(vector<Ast::Node> &node);
	static optional<Ast::Node> AppendToList(vector<Ast::Node> &node);
};

} // namespace shell
