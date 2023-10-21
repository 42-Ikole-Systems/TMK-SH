#pragma once

#include "shell/utility/types.hpp"

namespace shell {

class Environment {
public:
	virtual string get(const string &name) const = 0;
	virtual void add(const string &variable) = 0;
	virtual void remove(const string &name) = 0;
	virtual void exportVariable(const string &variable) = 0;
	virtual bool isExported(const string &name) const = 0;

	virtual char *const *materialize() const = 0;
};

} // namespace shell
