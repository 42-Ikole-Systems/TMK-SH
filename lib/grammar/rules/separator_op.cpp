#include "grammar/rules/separator_op.hpp"

namespace shell {

Rule SeparatorOp::make() {
	return Rule {
	    SeparatorOp::options,
	};
}

vector<Rule::Option> SeparatorOp::options() {
	return {Rule::Terminal {semicolon}, Rule::Terminal {ampersand}};
}

optional<Ast::Node> SeparatorOp::semicolon(Provider<Token> &tokens) {
	auto token = tokens.peek();
	if (token.type != Token::Type::Operator) {
		return nullopt;
	}
	auto &operator_token = token.operator_token;
	if (operator_token.type != OperatorToken::Type::Semicolon) {
		return nullopt;
	}
	auto node = Ast::SeparatorOp();
	node.left = nullptr;
	node.right = nullptr;
	tokens.consume();
	return node;
}

optional<Ast::Node> SeparatorOp::ampersand(Provider<Token> &tokens) {
	auto token = tokens.peek();
	if (token.type != Token::Type::Operator) {
		return nullopt;
	}
	auto &operator_token = token.operator_token;
	if (operator_token.type != OperatorToken::Type::Ampersand) {
		return nullopt;
	}
	auto node = Ast::SeparatorOp();
	node.left = nullptr;
	node.right = nullptr;
	tokens.consume();
	return node;
}

} // namespace shell
