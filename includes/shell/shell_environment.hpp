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
			list<shared_ptr<const char[]>>::iterator it;
			string_view value_part;
			bool exported;
			// Is this fetched from the char **environ
			bool is_base;
		};

	public:
		unordered_map<string_view, VariableEntry> environment;
		list<shared_ptr<const char[]>> owned_strings;

	public:
		const char *get(const string &name) const;
		void add(const string &variable);
		// UNSAFE, DOES NOT MAKE A COPY
		void addUnsafe(const char *variable);
		void remove(const string &name);
		void update(const string &variable);
		void markAsExported(const string &name);
		bool isExported(const string &name) const;
		MaterializedEnvironment materialize();

	private:
		void addInternal(const string &variable, bool exported = false);

	private:
		void remove(string_view name);
		shared_ptr<const char[]> copyVariable(const string &variable);
		string_view extractKey(const char *variable);
		std::pair<string_view, string_view> getKeyValueParts(const char *str);
	};

public:
	MaterializedEnvironment materialize() override;

	void add(const string &variable) override;

	void remove(const string &name) override;

	string get(const string &name) const override;

	void exportVariable(const string &variable) override;

	bool isExported(const string &name) const override;

private:
	VariableMap variable_map;
	MaterializedEnvironment cached_environment;
	bool modified = true;
};

} // namespace shell
