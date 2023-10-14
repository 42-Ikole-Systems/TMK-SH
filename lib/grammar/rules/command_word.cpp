#include "shell/grammar/rules/command_word.hpp"

namespace shell {

Rule CommandWord::make() {
	return Rule {
	    CommandWord::options,
	};
}

vector<Rule::Option> CommandWord::options() {
	// TODO: implement
	return {};
}

} // namespace shell
