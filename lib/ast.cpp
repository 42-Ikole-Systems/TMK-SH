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
Ast::Node::Node(Redirection &&redirection) : type(Type::Redirection), variant(std::move(redirection)) {
}
Ast::Node::Node(List &&list) : type(Type::List), variant(std::move(list)) {
}

Ast::Literal::Literal(Literal &&other) : token(std::move(other.token)) {
}
Ast::Literal::Literal(Token &&token) : token(token) {
}
Ast::Literal::~Literal() {
}

Ast::Command::Command(Command &&other) : program_name(other.program_name), arguments(std::move(other.arguments)) {
}
Ast::Command::Command(const string &program_name, List &&arguments)
    : program_name(program_name), arguments(std::move(arguments)) {
}
Ast::Command::~Command() {
}

Ast::SeparatorOp::SeparatorOp(SeparatorOp &&other) : left(std::move(other.left)), right(std::move(other.right)) {
}
Ast::SeparatorOp::~SeparatorOp() {
}

Ast::Redirection::Redirection(Redirection &&other)
    : file_name(other.file_name), redirection_type(other.redirection_type), io_number(other.io_number) {
}
Ast::Redirection::~Redirection() {
}

Ast::List::List(List &&other) : entries(std::move(other.entries)) {
}

Ast::List::List(Node node) : entries() {
	entries.push_back(std::move(node));
}

Ast::List::List(vector<Node> nodes) : entries(std::move_iterator(nodes.begin()), std::move_iterator(nodes.end())) {
}

void Ast::List::append(Node node) {
	entries.insert(entries.begin(), std::move(node));
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
		case Type::Redirection:
			get<Redirection>().print(level, logger);
			break;
		case Type::List:
			get<List>().print(level, logger);
			break;
	}
}

void Ast::Literal::print(int level, BLogger &logger) const {
	logger << "Literal:\n";
	printLevel(level + 1, logger);
	logger << ("Token: ");
	logger << token.toString() << "\n";
}

void Ast::Command::print(int level, BLogger &logger) const {
	logger << "Command:\n";
	printLevel(level + 1, logger);
	logger << "Name: " << program_name << "\n";
	printLevel(level + 1, logger);
	arguments.print(level + 1, logger);
}

void Ast::List::print(int level, BLogger &logger) const {
	logger << "List:\n";
	printLevel(level + 1, logger);
	logger << "Entries: [\n";
	for (auto &entry : entries) {
		entry.print(level + 2, logger);
	}
	printLevel(level + 1, logger);
	logger << "]\n";
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

void Ast::Redirection::print(int level, BLogger &logger) const {
	logger << "Redirection:\n";
	printLevel(level + 1, logger);
	logger << "Filename: %\n", file_name;
	printLevel(level + 1, logger);
	logger << "Redirection Type: %\n", Token::getOperatorString(redirection_type);
	printLevel(level + 1, logger);
	logger << "Filedescriptor: %\n", io_number;
}

} // namespace shell