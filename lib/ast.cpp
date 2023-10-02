#include "ast.hpp"
#include "print.hpp"

namespace shell {

Ast::Ast(unique_ptr<Node> root): root(std::move(root)) {}

void Ast::print() const {
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

void Ast::Command::print(int level) const {
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

void Ast::Sequence::print(int level) const {
    tprintf("Sequence:\n");
    if (left == nullptr) {
        tprintf("<null>\n");
    } else {
        left->print(level + 1);
    }
    if (right == nullptr) {
        tprintf("<null>\n");
    } else {
        right->print(level + 1);
    }
}

}