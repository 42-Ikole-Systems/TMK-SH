#pragma once

#include "grammar/rule.hpp"

namespace shell {

struct RuleProducer {
public:
	using grammar_func_t = Rule::grammar_func_t;

public:
	template <typename... Targs>
	static Rule::Option MakeOption(grammar_func_t node_assembler) {
		// Use fold expression to populate the sequence vector.
		return Rule::NonTerminal(node_assembler, {Targs::make()...});
	}
};

} // namespace shell
