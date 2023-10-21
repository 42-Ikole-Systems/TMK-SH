#pragma once

#include "shell/utility/types.hpp"

namespace shell {

struct EnvironmentVariables {
	// The char** holding the variables
	shared_ptr<char *const[]> map;
	// The strings that make up the data of the environment
	list<shared_ptr<const char[]>> owned_strings;
};

using MaterializedEnvironment = shared_ptr<EnvironmentVariables>;

class Environment {
public:
	virtual string get(const string &name) const = 0;
	virtual void add(const string &variable) = 0;
	virtual void remove(const string &name) = 0;
	virtual void exportVariable(const string &variable) = 0;
	virtual bool isExported(const string &name) const = 0;
	virtual MaterializedEnvironment materialize() = 0;
};

} // namespace shell
