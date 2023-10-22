
#pragma once

#include "shell/ast.hpp"
#include "executor.hpp"
#include <functional>

namespace shell {
class Builtin {
	using builtinFunction = std::function<ResultCode(const Ast::Command &, Environment &)>;

	Builtin() = delete;

	static std::map<string, builtinFunction> createBuiltinLookup();

public:
	/*!
	 * @brief If program is a builtin returns function object.
	 * @param program
	 */
	static optional<builtinFunction> getBuiltin(const string &program);

	/*!
	 * @brief exits program with code.
	 * @param code
	 */
	[[noreturn]] static void exit(ResultCode code);
	[[noreturn]] static void exit(const vector<string> &code);

	/*!
	 * @brief prints args to output.
	 * @param args
	 */
	static ResultCode echo(const vector<string> &args);

	static ResultCode export_variable(const vector<string> &args, Environment &env);
	static ResultCode set_variable(const vector<string> &args, Environment &env);
	static ResultCode unset_variable(const vector<string> &args, Environment &env);
	static ResultCode get_variable(const vector<string> &args, Environment &env);
};
} // namespace shell
