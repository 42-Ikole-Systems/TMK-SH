
#include "shell/environment.hpp"

#include <iostream>
#include <cstring>

namespace shell
{

EnvironmentVariables::EnvironmentVariables(const map<string, string>& variables) : envp(nullptr)
{
	const auto listSize = variables.size() + 1;
	try {
		envp = new char*[listSize];

		size_t i = 0;
		for (const auto& [name, value] : variables) {
			envp[i] = nullptr;
			char* variable = new char[name.size() + value.length() + 2]; // +2 for '=' and '/0'
			std::memmove(variable, name.data(), name.length());
			std::memmove(variable + name.length(), "=", 1);
			std::memmove(variable + name.length() + 1, value.data(), value.length() + 1); // +1 to include '/0'
			i++;
		}
	}
	catch (const std::exception& e)
	{
		destroy();
		throw e;
	}
}

EnvironmentVariables::~EnvironmentVariables()
{
	destroy();
}

void EnvironmentVariables::destroy()
{
	if (envp)
	{
		while (*envp != nullptr)
		{
			delete[] (*envp);
			envp++;
		}
		delete[] envp;
		envp = nullptr;
	}
}

char* const* EnvironmentVariables::raw() const
{
	return envp;
}

Environment& Environment::getInstance()
{
	static Environment environment;
	return environment;
}

void Environment::setEnvironmentVariables(char *const *envp)
{
	auto environment = getInstance();
	for (; *envp != nullptr; envp++)
	{
		const string variable = *envp;

		const auto splitPoint = variable.find('=');
		assert(splitPoint != string::npos);

		const auto name = variable.substr(0, splitPoint);
		const auto value = variable.substr(splitPoint + 1);
		environment.variables[name] = value;
	}
}

EnvironmentVariables Environment::getEnvironmentVariables()
{
	const auto environment = getInstance();
	return EnvironmentVariables(environment.variables);
}

void Environment::addEnvironmentVariable(const string& name, const string& value)
{
	auto environment = getInstance();
	environment.variables[name] = value;
}

void Environment::removeEnvironmentVariable(const string& name)
{
	auto environment = getInstance();
	environment.variables.erase(name);
}

const string& Environment::get(const string& name)
{
	auto environment = getInstance();
	return environment.variables.at(name);
}

} // namespace shell
