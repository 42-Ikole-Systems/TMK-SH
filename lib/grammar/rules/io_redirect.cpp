#include "shell/grammar/rules/io_redirect.hpp"

namespace shell {

Rule IORedirect::make() {
	return Rule {
	    IORedirect::options,
	};
}

vector<Rule::Option> IORedirect::options() {
	// TODO: implement
	return {};
}

} // namespace shell
