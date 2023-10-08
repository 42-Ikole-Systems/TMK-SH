#include "shell/executor/executor.hpp"
#include "shell/ast.hpp"
#include "shell/assert.hpp"

namespace shell {

ResultCode Executor::execute(Ast::SeparatorOp &op) {
	D_ASSERT(op.left != nullptr);
	D_ASSERT(op.right != nullptr);
	ResultCode result = execute(*op.left);
	if (result != ResultCode::Ok) {
		// ignore?
	}
	return execute(*op.right);
}

} // namespace shell
