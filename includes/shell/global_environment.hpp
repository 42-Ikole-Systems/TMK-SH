#pragma once

#include "shell/utility/types.hpp"
#include "shell/environment.hpp"

namespace shell {

class GlobalEnvironment : public Environment {
public:
	GlobalEnvironment() = default;

public:
	string get(const string &name) const override;
	void add(const string &value) override;
	void remove(const string &name) override;
	void setExportTag(const string &name) override;

	char *const *materialize() const override;

public:
	string_view extractKey(const char *variable);
	std::pair<string_view, string_view> getKeyValueParts(const char *str);
};

} // namespace shell
