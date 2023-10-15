#pragma once

#include "shell/utility/types.hpp"

namespace shell {

namespace Environment {

	/*!
	 * @brief
	 * @return
	 */
	char* const* getEnvironmentVariables();

	/*!
	 * @brief Adds variable to the environment variables, will overwrite if name already exits.
	 * @param variable In the form `name=value`
	 */
	void addEnvironmentVariable(const string& variable);

	/*!
	 * @brief
	 * @param name
	 */
	void removeEnvironmentVariable(const string &name);

	/*!
	 * @brief Gets value of environment variable.
	 * @param name
	 * @return
	 */
	const char* get(const string &name);

}; // namespace environment

} // namespace shell