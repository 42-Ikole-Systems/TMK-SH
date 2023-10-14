#pragma once

#include "shell/grammar/rule.hpp"
#include <optional>
#include "shell/ast.hpp"

namespace shell {

class IOFile {
public:
	static Rule make();
	static vector<Rule::Option> options();

private:
	static optional<Ast::Node> handler(vector<Ast::Node> &args);

	template <Token::Type REDIRECTION_TYPE>
	static Rule::Option CreateOption() {
		return Rule::NonTerminal(IOFile::handler, {GrammarUtil::ConsumeIf<REDIRECTION_TYPE>::make(), Filename::make()})
	}
};

} // namespace shell
