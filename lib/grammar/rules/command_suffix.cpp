#include "shell/grammar/rules/command_suffix.hpp"

namespace shell {

Rule CommandSuffix::make() {
	return Rule {
	    CommandSuffix::options,
	};
}

vector<Rule::Option> CommandSuffix::options() {
	// TODO: implement
	return {};
}

} // namespace shell
