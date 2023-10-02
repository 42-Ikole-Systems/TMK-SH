#include "parser.hpp"
#include "print.hpp"
#include <stdexcept>
#include <utility>

namespace shell {

Parser::Parser(Lexer& lexer): lexer(lexer) {}

Parser::Sequence::Sequence() : left(nullptr), right(nullptr) {}
Parser::Sequence::Sequence(Node* left, Node* right) : left(left), right(right) {}
Parser::Sequence::Sequence(Sequence&& other) : left(std::move(other.left)), right(std::move(other.right)) {}
Parser::Sequence::~Sequence() {
    delete left;
    delete right;
}

Parser::Command::Command() {}
Parser::Command::Command(string&& program, vector<string>&& args) : program(std::move(program)), args(std::move(args)) {}
Parser::Command::Command(Command&& other) : Command(std::move(other.program), std::move(other.args)) {}


Parser::Node::Node(Command&& command): type(Type::Command), command(std::move(command)) {}
Parser::Node::Node(Sequence&& sequence): type(Type::Sequence), sequence(std::move(sequence)) {}

Parser::Ast::Ast(unique_ptr<Parser::Node> root): root(std::move(root)) {}

/*
a ; b ; c ; d => Sequence (
    Command(a),
    Sequence(
        Command(b),
        Sequence(
            Command(c),
            Command(d)
        )
    )
)
Basically: command becomes left side of next sequence, and that sequence becomes the right side of the previous sequence.
Or the command becomes the right side of the previous sequence.
If there is no next or previous sequence, it is simply a command.
If the separatorOp == &, the command becomes `Detach` NodeType, which has Command as a child
*/
Parser::Ast Parser::getNextCommand() {
    while (lexer.peek().has_value()) {
        Token token = lexer.consume().value();
        token.print();
    }
    return Ast(nullptr);
}

void Parser::Ast::print() const {
    if (root == nullptr) {
        tprintf("<null>\n");
        return;
    }
    root->print(0);
}

static void printLevel(int level) {
    for (int i = 0; i < level; i++) {
        tprintf("  ");
    }
}

void Parser::Node::print(int level) const {
    printLevel(level);
    switch (type) {
        case Parser::Node::Type::Command:
            command.print(level);
            break;
        case Parser::Node::Type::Sequence:
            sequence.print(level);
            break;
    }
}

void Parser::Command::print(int level) const {
    tprintf("Command:\n");
    printLevel(level + 1);
    tprintf("Args: [ ");
    for (int i = 0; i < (int)args.size(); i++) {
        if (i != 0) {
            tprintf(", ");
        }
        tprintf("%", args[i]);
    }
    tprintf(" ]\n");
}

void Parser::Sequence::print(int level) const {
    tprintf("Sequence:\n");
    left->print(level + 1);
    right->print(level + 1);
}

}
