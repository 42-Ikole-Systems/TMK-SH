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

	struct List {
	public:
		List() = default;
		List(List &&other);
		List(Node node);
		List(vector<Node> nodes);

	public:
		void append(Node node);
		void print(int level) const;

	public:
		list<Node> entries;
	};

	struct Command {
		Command(Command &&other);
		Command(const string &program_name, List &&arguments);
		~Command();

		void print(int level) const;
		string program_name;
		List arguments;
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

	public:
		string file_name;
		Token::Type redirection_type;
		int32_t io_number;
	};

	class Node {
	public:
		enum class Type { SeparatorOp, Command, Literal, Redirection, List };

	private:
		Type type;
		std::variant<Command, SeparatorOp, Literal, Redirection, List> variant;

	public:
		Node(Node &&other);
		Node(Command &&command);
		Node(SeparatorOp &&separator_op);
		Node(Literal &&separator_op);
		Node(Redirection &&separator_op);
		Node(List &&list);

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
