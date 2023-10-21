#pragma once

#include "shell/utility/types.hpp"
#include "shell/interfaces/environment.hpp"

namespace shell {

class GlobalEnvironment : public Environment {
public:
	GlobalEnvironment();

public:
	string get(const string &name) const override;
	void add(const string &variable) override;
	void remove(const string &name) override;
	void exportVariable(const string &variable) override;
	bool isExported(const string &name) const override;

	MaterializedEnvironment materialize() override;

public:
	string_view extractKey(const char *variable);
	std::pair<string_view, string_view> getKeyValueParts(const char *str);

private:
	struct VariableData {
	public:
		VariableData(bool exported = false) : exported(exported) {
		}

	public:
		bool exported;
	};

private:
	void addInternal(const string &variable, bool exported = false);

private:
	unordered_map<string, VariableData> variables;
};

} // namespace shell
