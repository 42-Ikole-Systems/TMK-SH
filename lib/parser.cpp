#include "parser.hpp"
#include "print.hpp"
#include <stdexcept>
#include <utility>

namespace shell {

Parser::Parser(Provider<optional<Token>>& tokens): tokens(tokens), token_position(0) {}

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
    while ((fetched.size() - token_position) < (n + 1)) {
        auto next = tokens.consume();
        if (!next.has_value()) {
            token_position = fetched.size();
            return false;
        }
        fetched.emplace_back(next.value());
    }
    return true;
}

size_t Parser::saveState() {
    return token_position;
}

void Parser::setState(size_t state) {
    token_position = state;
}

void Parser::setParsedNode(unique_ptr<Ast::Node> node) {
    parsed = std::move(node);
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
Basically: command becomes left side of next separatorOp, and that separatorOp becomes the right side of the previous separatorOp.
Or the command becomes the right side of the previous separatorOp.
If there is no next or previous separatorOp, it is simply a command.
If the separatorOp == &, the command becomes `Detach` NodeType, which has Command as a child
*/
Ast Parser::getNextCommand() {
    if (!parseList()) {
        return Ast(nullptr);
    }
    return Ast(std::move(parsed));
}

bool Parser::parseList() {
    bool result = parseCommand();
    if (!result) {
        return false;
    }
    auto command = std::move(parsed);
    auto state = saveState();
    if (parseSeparatorOp()) {
        unique_ptr<Ast::Node> op = std::move(parsed);
        if (!parseList()) {
            // not an error: undo operator parsing
            setState(state);
            setParsedNode(std::move(command));
        } else {
            auto list = std::move(parsed);
            Ast::SeparatorOp* separator = dynamic_cast<Ast::SeparatorOp*>(op.get());
            separator->left = std::move(command);
            separator->right = std::move(list);
            setParsedNode(std::move(op));
        }
    } else {
        setParsedNode(std::move(command));
    }
    return true;
}

bool Parser::parseSeparatorOp() {
    auto token = peekToken();
    if (!token.has_value()) {
        return false;
    }
    if (token->type != Token::Type::Operator ||
        !(token->operator_token.type == OperatorToken::Type::Semicolon || token->operator_token.type == OperatorToken::Type::Ampersand)) {
        return false;
    }
    setParsedNode(make_unique<Ast::SeparatorOp>());
    consumeToken();
    return true;
}

// just parses a list of words for now
bool Parser::parseCommand() {
    vector<string> words;
    while (true) {
        auto token = peekToken();
        if (!token.has_value() || token->type != Token::Type::Word) {
            break;
        }
        words.emplace_back(token->word_token.value);
        consumeToken();
    }
    if (words.empty()) {
        return false;
    }
    setParsedNode(make_unique<Ast::Command>(std::move(words)));
    return true;
}

}
