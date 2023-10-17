#include "shell/grammar/rules/and_or.hpp"
#include "shell/grammar/rules/simple_command.hpp"
#include "shell/interfaces/provider.hpp"
#include "shell/grammar/grammar_util.hpp"
#include "shell/assert.hpp"

namespace shell {

Rule AndOr::make() {
	return Rule {
	    AndOr::options,
	};
}

vector<Rule::Option> AndOr::options() {
	return {GrammarUtil::MakeOption<SimpleCommand>(handler)};
}

optional<Ast::Node> AndOr::handler(vector<Ast::Node> &args) {
	D_ASSERT(args.size() == 1);
	return std::move(args[0]);
}

} // namespace shell
