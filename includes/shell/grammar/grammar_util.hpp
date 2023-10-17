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
			return Rule {[]() -> vector<Rule::Option> {
				return {Rule::Terminal {ConsumeIf<TOKEN_TYPE>::OP}};
			}};
		}

	public:
		static optional<Ast::Node> OP(TokenProvider &tokens) {
			auto token = tokens.consumeIf([](Token &token) { return token.getType() == TOKEN_TYPE; });
			if (!token.has_value()) {
				return nullopt;
			}
			return Ast::Literal(std::move(token.value()));
		}
	};
};

} // namespace shell
