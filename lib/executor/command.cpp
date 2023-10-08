#include "shell/executor/executor.hpp"
#include "shell/ast.hpp"
#include "shell/assert.hpp"
#include "shell/shell.hpp"
#include <unistd.h>
#include <stdlib.h>

namespace shell {

/*

https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html#tag_02_09_01_01

Command search and execution

(2) fork + execve

*/

static unique_ptr<char *const []> convertArguments(const vector<string> &vec) {
	std::unique_ptr<const char *[]> result(new const char *[vec.size() + 1]);
	for (size_t i = 0; i < vec.size(); i++) {
		result[i] = vec[i].c_str();
	}
	result[vec.size()] = nullptr;
	return unique_ptr<char *const[]>((char *const *)result.release());
}

ResultCode Executor::execute(Ast::Command &command) {
	D_ASSERT(!command.args.empty());

	const string &program = command.args[0];

	// todo: add builtin support
	// todo: path resolution
	D_ASSERT(program.find('/') != string::npos);

	pid_t pid = fork();
	if (pid < 0) {
		SYSCALL_ERROR("fork"); // <bash>: <fork>: strerror()
		return ResultCode::GeneralError;
	} else if (pid == 0) {
		// Child
		auto args = convertArguments(command.args);
		execve(program.c_str(), args.get(), envp);
		SYSCALL_ERROR("execve");
		if (errno == ENOEXEC) {
			Exit(ResultCode::CommandNotExecutable);
		}
		Exit(ResultCode::GeneralError);
	} else {
		// Parent
		// extract exit status from child
		if (waitpid(pid, NULL, 0) < 0) {
			SYSCALL_ERROR("fork");
		}
	}
	return ResultCode::Ok;
}

} // namespace shell
