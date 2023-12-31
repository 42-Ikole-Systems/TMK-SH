#include "shell/executor/executor.hpp"

#include "shell/logger.hpp"
#include "shell/utility/types.hpp"
#include <stdexcept>
#include <cassert>
#include <stdio.h>

namespace shell {

Executor::Executor(Environment &environment) : environment(environment) {
}

ResultCode Executor::execute(Ast::Node &node) {
	using NodeType = Ast::Node::Type;
	/*
	 node.expand() :: Sequence -> no-op
	 node.expand() :: Command -> expand args
	*/
	// expander.expand(node); // correct place to expand? `$(ls)`
	// "$( echo "abc" )" -> "abc" -> abc
	switch (node.getType()) {
		case NodeType::SeparatorOp: {
			return execute(node.get<Ast::SeparatorOp>());
		}
		case NodeType::Command: {
			return execute(node.get<Ast::Command>());
		}
		default: {
			throw std::runtime_error("Unimplemented type for Ast::Node in Executor::execute");
		}
	}
}

ResultCode Executor::execute(Ast &ast) {
	if (ast.root == nullptr) {
		return ResultCode::Ok;
	}
	return execute(*ast.root);
}

}; // namespace shell
