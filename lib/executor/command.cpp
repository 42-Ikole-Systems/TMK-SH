#include "shell/executor/executor.hpp"
#include "shell/ast.hpp"
#include "shell/assert.hpp"
#include "shell/shell.hpp"
#include "shell/interfaces/environment.hpp"
#include "shell/utility/split.hpp"
#include "shell/executor/builtins.hpp"

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <ranges>
#include <filesystem>

#include <sys/stat.h>

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

static bool isExecutable(const string &filepath) {
	const auto &permissions = std::filesystem::status(filepath).permissions();
	return (bool)(permissions & std::filesystem::perms::owner_exec) ||
	       (bool)(permissions & std::filesystem::perms::group_exec) ||
	       (bool)(permissions & std::filesystem::perms::others_exec);
}

/*!
 * @brief Resolves program path.
 * @param command
 */
optional<string> Executor::resolvePath(const string &program) {
	if (program.find('/') != string::npos) {
		if (std::filesystem::exists(program)) {
			return program;
		} else {
			return nullopt;
		}
	}

	const auto &path = environment.get("PATH");
	if (!path.has_value()) {
		return nullopt;
	}
	for (const auto location : LazySplit(path.value(), ":")) {
		const auto program_path = std::filesystem::path(location) / program;
		if (std::filesystem::exists(program_path) && isExecutable(program_path)) {
			return program_path.string();
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
		const auto args = convertArguments(command);
		const auto env = environment.materialize();
		execve(programPath.value().c_str(), args.get(), env->map.get());
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
	const auto &program = command.program_name;

	auto builtin = Builtin::getBuiltin(program);
	if (builtin.has_value()) {
		return (*builtin)(command, environment);
	} else {
		return executeProcess(program, command);
	}
}

} // namespace shell
