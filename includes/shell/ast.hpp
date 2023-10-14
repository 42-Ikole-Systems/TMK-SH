#pragma once

#include "shell/util.hpp"
#include <variant>
#include "shell/lexer/token.hpp"

namespace shell {

class Ast {
public:
	class Node;

	struct Literal {
		Literal(Literal &&other);
		Literal(Token &&token);
		~Literal();

		void print(int level) const;
		Token token;
	};

	struct Command {
		Command(Command &&other);
		Command(vector<string> &&args);
		~Command();

		void print(int level) const;
		vector<string> args; // args[0] == program name
	};

	struct SeparatorOp {
		SeparatorOp() = default;
		SeparatorOp(SeparatorOp &&other);
		~SeparatorOp();

		void print(int level) const;
		unique_ptr<Node> left;
		unique_ptr<Node> right;
	};

	struct Redirection {
	public:
		Redirection() = default;
		Redirection(Redirection &&other);
		~Redirection();

	public:
		void print(int level) const;
		unique_ptr<Node> left;
		unique_ptr<Node> right;
		Token::Type redirection_type;
	};

	class Node {
	public:
		enum class Type { SeparatorOp, Command, Literal, Redirection };

	private:
		Type type;
		std::variant<Command, SeparatorOp, Literal, Redirection> variant;

	public:
		Node(Node &&other);
		Node(Command &&command);
		Node(SeparatorOp &&separator_op);
		Node(Literal &&separator_op);
		Node(Redirection &&separator_op);

		Type getType() const;

		template <typename T>
		const T &get() const {
			return std::get<T>(variant);
		}

		template <typename T>
		T &get() {
			return std::get<T>(variant);
		}

		void print(int level) const;
	};

public:
	Ast(unique_ptr<Node> root);

	unique_ptr<Node> root;

	void print() const;
};

} // namespace shell
