#include "shell/executor/executor.hpp"
#include "shell/ast.hpp"
#include "shell/assert.hpp"
#include "shell/shell.hpp"
#include "shell/environment.hpp"
#include "shell/utility/split.hpp"

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

static unique_ptr<char *const []> convertArguments(const Ast::List &list) {
	auto &vec = list.entries;
	std::unique_ptr<const char *[]> result(new const char *[vec.size() + 1]);
	size_t i = 0;
	for (auto &entry : vec) {
		if (entry.getType() != Ast::Node::Type::Literal) {
			throw NotImplementedException("Execution of Command with non-literal arguments not supported yet");
		}
		D_ASSERT(entry.getType() == Ast::Node::Type::Literal);
		result[i] = entry.get<Ast::Literal>().token.get<WordToken>().value.c_str();
		i++;
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
		return program;
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
		auto args = convertArguments(command.arguments);
		execve(programPath.value().c_str(), args.get(), Environment::getEnvironmentVariables());
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
