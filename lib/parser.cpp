#include "shell/parser.hpp"
#include "shell/print.hpp"
#include <stdexcept>
#include <utility>
#include "shell/grammar/rules/list_rule.hpp"
#include "shell/assert.hpp"

namespace shell {

optional<Token> BufferedTokenProvider::peek() {
	return parser.peekToken();
}

optional<Token> BufferedTokenProvider::consume() {
	return parser.consumeToken();
}

Parser::Parser(TokenProvider &tokens) : tokens(tokens), token_position(0) {
}

// Warning: nullptr if no more tokens left
optional<Token> Parser::peekToken() {
	if (!nextToken()) {
		return nullopt;
	}
	return fetched.at(token_position);
}

optional<Token> Parser::consumeToken() {
	if (!nextToken()) {
		return nullopt;
	}
	return fetched.at(token_position++);
}

bool Parser::nextToken() {
	if (unconsumedTokens() != 0) {
		return true;
	}
	auto next = tokens.consume();
	if (!next.has_value()) {
		return false;
	}
	fetched.emplace_back(std::move(next.value()));
	return true;
}

size_t Parser::unconsumedTokens() {
	return fetched.size() - token_position;
}

size_t Parser::saveState() {
	return token_position;
}

void Parser::setState(size_t state) {
	token_position = state;
}

/*
a ; b ; c ; d => SeparatorOp (
    Command(a),
    SeparatorOp
        Command(b),
        SeparatorOp(
            Command(c),
            Command(d)
        )
    )
)
*/

optional<Ast::Node> Parser::parse(Rule::NonTerminal &option) {
	vector<Ast::Node> nodes;
	auto &rules = option.sequence;
	auto state = saveState();
	for (auto &rule : rules) {
		auto result = parse(rule);
		if (!result.has_value()) {
			// Parsing this rule failed, revert back the state
			setState(state);
			return nullopt;
		}
		nodes.push_back(std::move(result.value()));
	}
	// Sequence was parsed successfully, assemble the Node from the components
	auto result = option.node_assembler(nodes);
	// If all the components of a rule pass, the node assembly should also pass
	D_ASSERT(result.has_value());
	return result;
}

BufferedTokenProvider Parser::tokenProvider() {
	return BufferedTokenProvider(*this);
}

optional<Ast::Node> Parser::parse(Rule::Terminal &option) {
	auto state = saveState();
	auto token_provider = tokenProvider();
	auto result = option.token_consumer(token_provider);
	if (!result.has_value()) {
		setState(state);
		return nullopt;
	}
	return result;
}

optional<Ast::Node> Parser::parse(Rule rule) {
	using OptionType = Rule::Option::Type;
	auto options = rule.options();
	for (auto &option : options) {
		auto type = option.getType();
		// Save the state so we can revert it if the rule doesn't match
		auto state = saveState();
		switch (type) {
			case OptionType::NonTerminal: {
				auto &non_terminal = option.get<Rule::NonTerminal>();
				auto result = parse(non_terminal);
				if (result.has_value()) {
					return result;
				}
				break;
			}
			case OptionType::Terminal: {
				auto &terminal = option.get<Rule::Terminal>();
				auto result = parse(terminal);
				if (result.has_value()) {
					return result;
				}
				break;
			}
			default: {
				throw std::runtime_error("Unrecognized option for OptionType in Parser::parse(Rule rule)");
			}
		}
		// The option didn't return a valid result, reset the state
		setState(state);
	}
	return nullopt;
}

Ast Parser::parse() {
	auto initial_rule = ListRule::make();
	auto root_node = parse(std::move(initial_rule));

	if (!root_node.has_value()) {
		return Ast(nullptr);
	}
	return Ast(make_unique<Ast::Node>(std::move(root_node.value())));
}

} // namespace shell
