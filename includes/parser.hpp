#pragma once

#include "lexer/token.hpp"
#include "lexer/lexer.hpp"
#include "util.hpp"

namespace shell {


class Parser {
private:
	Lexer& lexer;

public:
	Parser(Lexer& lexer);

	struct Node;

	// have to use pointers/new+delete cuz it's a self-referential data structure
	struct Sequence {
		Sequence();
		Sequence(Node* left, Node* right);
		Sequence(Sequence&& sequence);
		~Sequence();

		Node* left;
		Node* right;

		void print(int level) const;
	};

	struct Command {
		Command();
		Command(string &&program, vector<string> &&args);
		Command(Command &&);

		string program;
		vector<string> args;

		void print(int level) const;
	};

	struct Node {
		enum class Type { Sequence, Command };

		Node(Command &&command);
		Node(Sequence &&sequence);

		Type type;
		Sequence sequence;
		Command command;

		void print(int level) const;
	};

	struct Ast {
		Ast(unique_ptr<Node> root);
		unique_ptr<Node> root;

		void print() const;
	};

	Ast getNextCommand();
};

} // namespace shell
