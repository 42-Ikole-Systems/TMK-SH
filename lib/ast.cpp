#include "ast.hpp"
#include "print.hpp"

namespace shell {

Ast::Ast(unique_ptr<Node> root) : root(std::move(root)) {
}

Ast::Node::Node(Node &&other) : type(other.type), variant(std::move(other.variant)) {
}
Ast::Node::Node(Command &&command) : type(Type::Command), variant(std::move(command)) {
}
Ast::Node::Node(SeparatorOp &&separator_op) : type(Type::SeparatorOp), variant(std::move(separator_op)) {
}

Ast::Command::Command(Command &&other) : args(std::move(other.args)) {
}
Ast::Command::Command(vector<string> &&args) : args(args) {
}
Ast::Command::~Command() {
}

Ast::SeparatorOp::SeparatorOp(SeparatorOp &&other) : left(std::move(other.left)), right(std::move(other.right)) {
}
Ast::SeparatorOp::~SeparatorOp() {
}

void Ast::print() const {
	if (root == nullptr) {
		tprintf("<null>\n");
		return;
	}
	root->print(0);
}

static void printLevel(int level) {
	for (int i = 0; i < level; i++) {
		tprintf("    ");
	}
}

void Ast::Node::print(int level) const {
	printLevel(level);
	switch (type) {
	case Type::Command:
		getCommand().print(level);
		break;
	case Type::SeparatorOp:
		getSeparatorOp().print(level);
		break;
	}
}

Ast::SeparatorOp &Ast::Node::getSeparatorOp() {
	return std::get<SeparatorOp>(variant);
}

const Ast::SeparatorOp &Ast::Node::getSeparatorOp() const {
	return std::get<SeparatorOp>(variant);
}

Ast::Command &Ast::Node::getCommand() {
	return std::get<Command>(variant);
}

const Ast::Command &Ast::Node::getCommand() const {
	return std::get<Command>(variant);
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

void Ast::SeparatorOp::print(int level) const {
	tprintf("SeparatorOp:\n");
	if (left == nullptr) {
		printLevel(level + 1);
		tprintf("<null>\n");
	} else {
		left->print(level + 1);
	}
	if (right == nullptr) {
		printLevel(level + 1);
		tprintf("<null>\n");
	} else {
		right->print(level + 1);
	}
}

} // namespace shell