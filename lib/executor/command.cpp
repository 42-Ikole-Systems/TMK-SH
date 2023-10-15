#include "shell/executor/executor.hpp"
#include "shell/ast.hpp"
#include "shell/assert.hpp"
#include "shell/shell.hpp"
#include "shell/environment.hpp"
#include "shell/utility/split.hpp"
#include "shell/executor/builtins.hpp"

#include <unistd.h>
#include <stdlib.h>
#include <ranges>
#include <filesystem>

namespace shell {

/*

https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html#tag_02_09_01_01

Command search and execution

(2) fork + execve

*/

static unique_ptr<char *const[]> convertArguments(const vector<string> &vec) {
	std::unique_ptr<const char *[]> result(new const char *[vec.size() + 1]);
	for (size_t i = 0; i < vec.size(); i++) {
		result[i] = vec[i].c_str();
	}
	result[vec.size()] = nullptr;
	return unique_ptr<char *const[]>((char *const *)result.release());
}

/*!
 * @brief Resolves program path.
 * @param command
 */
static optional<string> resolvePath(const string &program) {
	if (program.find('/') != string::npos) {
		if (std::filesystem::exists(program)) {
			return program;
		} else {
			return nullopt;
		}
	}

	const auto &path = Environment::get("PATH");
	for (const auto location : LazySplit(path, ":")) {
		const auto programPath = std::filesystem::path(location) / program;
		if (std::filesystem::exists(programPath)) {
			return programPath.string();
		}
	}
	return nullopt;
}

ResultCode Executor::executeProcess(const string &program, const Ast::Command &command) {
	const auto programPath = resolvePath(program);
	if (!programPath.has_value()) {
		LOG_ERROR("%: command not found\n", program);
		return ResultCode::CommandNotFound;
	}

	pid_t pid = fork();
	if (pid < 0) {
		SYSCALL_ERROR("fork"); // <bash>: <fork>: strerror()
		return ResultCode::GeneralError;
	} else if (pid == 0) {
		// Child
		auto args = convertArguments(command.args);
		execve(programPath.value().c_str(), args.get(), Environment::getEnvironmentVariables());
		SYSCALL_ERROR("execve");
		if (errno == ENOEXEC) {
			Builtin::exit(ResultCode::CommandNotExecutable);
		}
		Builtin::exit(ResultCode::GeneralError);
	} else {
		// Parent
		// extract exit status from child
		if (waitpid(pid, NULL, 0) < 0) {
			SYSCALL_ERROR("fork");
		}
	}
	return ResultCode::Ok;
}

ResultCode Executor::execute(Ast::Command &command) {
	D_ASSERT(!command.args.empty());
	const auto &program = command.args[0];

	auto builtin = Builtin::getBuiltin(program);
	if (builtin.has_value()) {
		return (*builtin)(command);
	} else {
		return executeProcess(program, command);
	}
}

} // namespace shell
