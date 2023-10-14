#include "shell/grammar/rules/command_prefix.hpp"

namespace shell {

Rule CommandPrefix::make() {
	return Rule {
	    CommandPrefix::options,
	};
}

vector<Rule::Option> CommandPrefix::options() {
	// TODO: implement
	return {};
}

} // namespace shell
