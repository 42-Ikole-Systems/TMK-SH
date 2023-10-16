#pragma once

#include "shell/utility/types.hpp"

namespace shell {

struct EnvironmentVariables {
	// The char** holding the variables
	shared_ptr<char *const> map;
	// The strings that make up the data of the environment
	list<shared_ptr<const char>> owned_strings;
};

using MaterializedEnvironment = shared_ptr<EnvironmentVariables>;

class Environment {
public:
public:
	virtual MaterializedEnvironment getEnvironmentVariables() = 0;

	/*!
	* @brief Adds variable to the environment variables, will overwrite if name already exits.
	* @param variable In the form `name=value`
	*/
	virtual void addEnvironmentVariable(const string &variable) = 0;

	/*!
	* @brief
	* @param name
	*/
	virtual void removeEnvironmentVariable(const string &name) = 0;

	/*!
	* @brief Gets value of environment variable.
	* @param name
	* @return
	*/
	virtual const char *get(const string &name) = 0;
	/*!
	* @brief
	* @return
	*/
};


} // namespace shell
