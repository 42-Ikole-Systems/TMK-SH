#pragma once

#include <optional>
#include "shell/utility/types.hpp"
#include "shell/interfaces/provider.hpp"
#include "shell/lexer/token_provider.hpp"
#include "shell/ast.hpp"

namespace shell {

struct Rule {
public:
	using grammar_func_t = std::function<optional<Ast::Node>(vector<Ast::Node> &nodes)>;
	using token_consumer_t = std::function<optional<Ast::Node>(TokenProvider &provider)>;

	struct NonTerminal {
		NonTerminal(grammar_func_t node_assembler, vector<Rule> sequence)
		    : node_assembler(node_assembler), sequence(std::move(sequence)) {
		}
		grammar_func_t node_assembler;
		vector<Rule> sequence;
	};

	struct Terminal {
		token_consumer_t token_consumer;
	};

	struct Option {
	public:
		enum class Type : uint8_t { NonTerminal, Terminal };

	public:
		Option(NonTerminal &&opt);
		Option(Terminal &&opt);

	public:
		Type type;
		variant<NonTerminal, Terminal> option;

	public:
		Type getType() const {
			return type;
		}

		template <typename T>
		const T &get() const {
			return std::get<T>(option);
		}

		template <typename T>
		T &get() {
			return std::get<T>(option);
		}
	};

	using options_func_t = std::function<vector<Option>()>;

public:
	options_func_t options;
};

} // namespace shell
