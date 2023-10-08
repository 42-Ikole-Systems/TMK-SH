#include "shell/ast.hpp"
#include "shell/print.hpp"

namespace shell {

Ast::Ast(unique_ptr<Node> root) : root(std::move(root)) {
}

Ast::Node::Node(Node &&other) : type(other.type), variant(std::move(other.variant)) {
}
Ast::Node::Node(Command &&command) : type(Type::Command), variant(std::move(command)) {
}
Ast::Node::Node(SeparatorOp &&separator_op) : type(Type::SeparatorOp), variant(std::move(separator_op)) {
}
Ast::Node::Node(Literal &&literal) : type(Type::Literal), variant(std::move(literal)) {
}

Ast::Literal::Literal(Literal &&other) : token(std::move(other.token)) {
}
Ast::Literal::Literal(Token &&token) : token(token) {
}
Ast::Literal::~Literal() {
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

Ast::Node::Type Ast::Node::getType() const {
	return type;
}

void Ast::print() const {
	BLogger logger(LogLevel::Debug);
	if (root == nullptr) {
		logger << "<null>\n";
		return;
	}
	root->print(0, logger);
}

static void printLevel(int level, BLogger &logger) {
	for (int i = 0; i < level; i++) {
		logger << "    ";
	}
}

void Ast::Node::print(int level, BLogger &logger) const {
	printLevel(level, logger);
	switch (type) {
		case Type::Command:
			get<Command>().print(level, logger);
			break;
		case Type::SeparatorOp:
			get<SeparatorOp>().print(level, logger);
			break;
		case Type::Literal:
			get<Literal>().print(level, logger);
			break;
	}
}

void Ast::Literal::print(int level, BLogger &logger) const {
	tprintf("Literal:\n");
	printLevel(level + 1, logger);
	tprintf("Token: ");
	logger << token.toString();
}

void Ast::Command::print(int level, BLogger &logger) const {
	tprintf("Command:\n");
	printLevel(level + 1, logger);
	tprintf("Args: [ ");
	for (int i = 0; i < (int)args.size(); i++) {
		if (i != 0) {
			logger << ", ";
		}
		logger << args[i];
	}
	logger << " ]\n";
}

void Ast::SeparatorOp::print(int level, BLogger &logger) const {
	logger << "SeparatorOp:\n";
	if (left == nullptr) {
		printLevel(level + 1, logger);
		logger << "<null>\n";
	} else {
		left->print(level + 1, logger);
	}
	if (right == nullptr) {
		printLevel(level + 1, logger);
		logger << "<null>\n";
	} else {
		right->print(level + 1, logger);
	}
}

} // namespace shell