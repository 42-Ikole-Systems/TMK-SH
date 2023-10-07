#include "parser.hpp"
#include "print.hpp"
#include <stdexcept>
#include <utility>

namespace shell {

Parser::Parser(Provider<optional<Token>> &tokens) : tokens(tokens), token_position(0) {
}

optional<Token> Parser::peekToken(size_t n) {
	if (!retrieveTokens(n)) {
		return nullopt;
	}
	return fetched.at(token_position + n);
}

optional<Token> Parser::consumeToken(size_t n) {
	if (!retrieveTokens(n)) {
		return nullopt;
	}
	token_position += n;
	return fetched.at(token_position++);
}

bool Parser::retrieveTokens(size_t n) {
	while (unconsumedTokens() < (n + 1)) {
		auto next = tokens.consume();
		if (!next.has_value()) {
			token_position = fetched.size();
			return false;
		}
		fetched.emplace_back(next.value());
	}
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
Basically: command becomes left side of next separatorOp, and that separatorOp becomes the right side of the previous
separatorOp. Or the command becomes the right side of the previous separatorOp. If there is no next or previous
separatorOp, it is simply a command. If the separatorOp == &, the command becomes `Detach` NodeType, which has Command
as a child
*/
Ast Parser::getNextCommand() {
	optional<Ast::Node> listNode = parseList();
	if (!listNode.has_value()) {
		return Ast(nullptr);
	}
	return Ast(make_unique<Ast::Node>(std::move(listNode.value())));
}

optional<Ast::Node> Parser::parseList() {
	auto command = parseCommand();
	if (!command.has_value()) {
		return nullopt;
	}
	auto state = saveState();
	optional<Ast::SeparatorOp> separator = parseSeparatorOp();
	if (!separator.has_value()) {
		return Ast::Node(std::move(command.value()));
	}

	optional<Ast::Node> list = parseList();
	if (!list.has_value()) {
		setState(state); // undo parsing of separator
		return Ast::Node(std::move(command.value()));
	}

	separator->left = make_unique<Ast::Node>(Ast::Node(std::move(command.value())));
	separator->right = make_unique<Ast::Node>(std::move(list.value()));
	return separator;
}

optional<Ast::SeparatorOp> Parser::parseSeparatorOp() {
	auto token = peekToken();
	if (!token.has_value()) {
		return nullopt;
	}
	if ((token->getType() != Token::Type::Semicolon && token->getType() != Token::Type::And)) {
		return nullopt;
	}
	consumeToken();
	return Ast::SeparatorOp();
}

// just parses a list of words for now
optional<Ast::Command> Parser::parseCommand() {
	vector<string> words;
	while (true) {
		auto token = peekToken();
		if (!token.has_value()) {
			break;
		} else if (token->getType() != Token::Type::Word) {
			break;
		}
		words.emplace_back(token->get<WordToken>().value);
		consumeToken();
	}
	if (words.empty()) {
		return nullopt;
	}
	return Ast::Command(std::move(words));
}

} // namespace shell
