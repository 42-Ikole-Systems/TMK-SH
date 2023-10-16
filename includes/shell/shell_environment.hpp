#pragma once

#include "shell/utility/types.hpp"
#include "shell/interfaces/environment.hpp"

namespace shell {

class ShellEnvironment : public Environment {
public:
	ShellEnvironment();
private:
	struct VariableMap {
	private:
		struct VariableEntry {
			// Save where in the 'owned_strings' this is stored, for removal
			list<shared_ptr<const char>>::iterator it;
			string_view value_part;
			// Is this fetched from the char **environ
			bool is_base;
		};
	public:
		unordered_map<string_view, VariableEntry> environ;
		list<shared_ptr<const char>> owned_strings;
	public:
		const char *get(const string &name);
		void add(const string &variable);
		// UNSAFE, DOES NOT MAKE A COPY
		void addUnsafe(const char *variable);
		void remove(const string &name);
		void update(const string &variable);
		MaterializedEnvironment materialize();
	private:
		shared_ptr<const char> copyVariable(const string &variable);
		string_view extractKey(const char *variable);
		std::pair<string_view, string_view> getKeyValueParts(const char *str);
	};
public:
	MaterializedEnvironment getEnvironmentVariables() override;

	void addEnvironmentVariable(const string &variable) override;

	void removeEnvironmentVariable(const string &name) override;

	const char *get(const string &name) override;
private:
	VariableMap variable_map;
	MaterializedEnvironment cached_environment;
	bool modified = true;
};

} // namespace shell
