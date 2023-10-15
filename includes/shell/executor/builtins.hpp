
#pragma once

#include "shell/ast.hpp"
#include "executor.hpp"

namespace shell
{
	/*!
	 * @brief Singleton class for builtins.
	*/
	class Builtin
	{
		using builtinFunction = std::function<ResultCode(const Ast::Command&)>;
		
		std::map<string, builtinFunction> builtins;
	
		Builtin();

		/*!
		 * @brief gets singleton instance.
		*/
		static Builtin& getInstance();

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
		[[noreturn]] static void exit(const vector<string>& code);

		/*!
		 * @brief prints args to output.
		 * @param args
		*/
		static ResultCode echo(const vector<string>& args);
	};
} // namespace shell
