#pragma once

#include "shell/utility/types.hpp"

namespace shell
{

/*!
 * @brief RAII wrapper for envp.
*/
class EnvironmentVariables
{
	char** envp;

	void destroy();

public:

	/*!
	 * @brief Constructs RAII envp from variables map.
	 * @param variables
	*/
	EnvironmentVariables(const map<string, string>& variables);

	/*!
	 * @brief -.
	*/
	~EnvironmentVariables();

	/*!
	 * @brief -.
	 * @return raw envp (Do not free / destroy!!)
	*/
	char* const* raw() const;
};

/*!
 * @brief Singleton environent.
*/
class Environment
{
public:

	map<string, string> variables; /*!< environment variables. */

	/*!
	 * @brief -.
	*/
	Environment() = default;

	/*!
	 * @brief -.
	*/
	static Environment& getInstance();

public:

	/*!
	 * @brief Sets Environment. (will overwrite if environment is already set)
	 * @param envp
	*/
	static void setEnvironmentVariables(char *const *envp);

	/*!
	 * @brief -.
	 * @return
	*/
	static EnvironmentVariables getEnvironmentVariables();

	/*!
	 * @brief Adds variable to the environment variables, will overwrite if name already exits.
	 * @param name
	 * @param value
	*/
	static void addEnvironmentVariable(const string& name, const string& value);

	/*!
	 * @brief -.
	 * @param name
	*/
	static void removeEnvironmentVariable(const string& name);

	/*!
	 * @brief Gets value of environment variable.
	 * @param name
	 * @return
	*/
	static const string& get(const string& name);
}; // class environment

} // namespace shell