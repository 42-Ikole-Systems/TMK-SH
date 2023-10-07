#pragma once

#include "parser.hpp"
#include <optional>

namespace shell {

struct GrammarRule {
public:
	using grammar_func_t = std::function<optional<Ast::Node>(const vector<Ast::Node> &nodes)>;

	struct Option {
		Option(grammar_func_t node_assembler, vector<GrammarRule> sequence)
		    : node_assembler(node_assembler), sequence(std::move(sequence)) {
		}
		grammar_func_t node_assembler;
		vector<GrammarRule> sequence;
	};

	template <typename... Targs>
	static Option MakeOption(grammar_func_t node_assembler) {
		// Use fold expression to populate the sequence vector.
		return Option(node_assembler, {Targs::make()...});
	}

	using options_func_t = std::function<vector<GrammarRule::Option>()>;

public:
	options_func_t options;
};

} // namespace shell
