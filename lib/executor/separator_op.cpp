#include "executor/executor.hpp"
#include "ast.hpp"
#include "assert.hpp"

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
