#include "shell/executor/executor.hpp"
#include "shell/ast.hpp"
#include "shell/assert.hpp"
#include "shell/shell.hpp"
#include "shell/interfaces/environment.hpp"
#include "shell/utility/split.hpp"

#include <sys/wait.h>
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

/*!
 * @brief Generates array of arguments for execve (first argument is program name).
 * @param command
 * @return
*/
static unique_ptr<char *const []> convertArguments(const Ast::Command &command) {
	const auto &vec = command.arguments.entries;
	std::unique_ptr<const char *[]> result(new const char *[vec.size() + 2]); // +1 for executable name, +1 for nullptr
	result[0] = command.program_name.c_str();
	std::cout << "program name: " << result[0] << std::endl;
	size_t i = 1;
	for (auto &entry : vec) {
		if (entry.getType() != Ast::Node::Type::Literal) {
			throw NotImplementedException("Execution of Command with non-literal arguments not supported yet");
		}
		D_ASSERT(entry.getType() == Ast::Node::Type::Literal);
		result[i] = entry.get<Ast::Literal>().token.get<WordToken>().value.c_str();
		i++;
	}
	result[i] = nullptr;
	return unique_ptr<char *const[]>((char *const *)result.release());
}

/*!
 * @brief Resolves program path.
 * @param command
 */
optional<string> Executor::resolvePath(const string &program) {
	if (program.find('/') != string::npos) {
		return program;
	}

	const auto &path = environment.get("PATH");
	for (const auto location : LazySplit(path, ":")) {
		const auto programPath = std::filesystem::path(location) / program;
		if (std::filesystem::exists(programPath)) {
			return programPath.string();
		}
	}
	return nullopt;
}

ResultCode Executor::execute(Ast::Command &command) {
	// todo: add builtin support
	const auto &program = command.program_name;
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
		const auto args = convertArguments(command);
		const auto env = environment.getEnvironmentVariables();
		execve(programPath.value().c_str(), args.get(), env->map.get());
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
