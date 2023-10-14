#pragma once

#include "shell/grammar/rule.hpp"

namespace shell {

struct GrammarUtil {
public:
	using grammar_func_t = Rule::grammar_func_t;

public:
	template <typename... Targs>
	static Rule::Option MakeOption(grammar_func_t node_assembler) {
		// Use fold expression to populate the sequence vector.
		return Rule::NonTerminal(node_assembler, {Targs::make()...});
	}

	template <Token::Type TOKEN_TYPE>
	struct ConsumeIf {
	public:
		static Rule make() {
			return Rule {
			    OP,
			};
		}

	public:
		static optional<Ast::Node> OP(TokenProvider &tokens) {
			auto token = tokens.peek();
			if (token->getType() != TOKEN_TYPE) {
				return nullopt;
			}
			tokens.consume();
			return Ast::Literal(std::move(token.value()));
		}
	};
};

} // namespace shell
