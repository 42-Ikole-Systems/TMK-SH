#include <catch.hpp>
#include "test_helpers.hpp"
#include "shell/lexer/reader_char_provider.hpp"
#include "shell/logger.hpp"

bool Equals(Ast::Node *lhs, Ast::Node *rhs);

bool ListEquals(Ast::List &lhs, Ast::List &rhs) {
	REQUIRE(lhs.entries.size() == rhs.entries.size());
	auto a_it = lhs.entries.begin();
	auto b_it = rhs.entries.begin();
	for (; a_it != lhs.entries.end();) {
		auto &a = *a_it;
		auto &b = *b_it;
		REQUIRE(Equals(&a, &b));
		a_it++;
		b_it++;
	}
	return true;
}

bool CommandEquals(Ast::Command &lhs, Ast::Command &rhs) {
	REQUIRE(ListEquals(lhs.arguments, rhs.arguments));
	REQUIRE(lhs.program_name == rhs.program_name);
	return true;
}

bool RedirectionEquals(Ast::Redirection &lhs, Ast::Redirection &rhs) {
	REQUIRE(lhs.file_name == rhs.file_name);
	REQUIRE(lhs.io_number == rhs.io_number);
	REQUIRE(lhs.redirection_type == rhs.redirection_type);
	return true;
}

bool SeparatorOpEquals(Ast::SeparatorOp &lhs, Ast::SeparatorOp &rhs) {
	REQUIRE(Equals(lhs.left.get(), rhs.left.get()));
	REQUIRE(Equals(lhs.right.get(), rhs.right.get()));
	return true;
}
bool LiteralEquals(Ast::Literal &lhs, Ast::Literal &rhs) {
	REQUIRE(lhs.token.getType() == rhs.token.getType());
	return true;
}

bool Equals(Ast::Node *lhs, Ast::Node *rhs) {
	if (!lhs) {
		REQUIRE(!rhs);
		return true;
	} else {
		REQUIRE(rhs);
	}
	REQUIRE(lhs->getType() == rhs->getType());
	switch (lhs->getType()) {
		case Ast::Node::Type::Command: {
			auto &a = lhs->get<Ast::Command>();
			auto &b = rhs->get<Ast::Command>();
			REQUIRE(CommandEquals(a, b));
			break;
		}
		case Ast::Node::Type::List: {
			auto &a = lhs->get<Ast::List>();
			auto &b = rhs->get<Ast::List>();
			REQUIRE(ListEquals(a, b));
			break;
		}
		case Ast::Node::Type::Literal: {
			auto &a = lhs->get<Ast::Literal>();
			auto &b = rhs->get<Ast::Literal>();
			REQUIRE(LiteralEquals(a, b));
			break;
		}
		case Ast::Node::Type::Redirection: {
			auto &a = lhs->get<Ast::Redirection>();
			auto &b = rhs->get<Ast::Redirection>();
			REQUIRE(RedirectionEquals(a, b));
			break;
		}
		case Ast::Node::Type::SeparatorOp: {
			auto &a = lhs->get<Ast::SeparatorOp>();
			auto &b = rhs->get<Ast::SeparatorOp>();
			REQUIRE(SeparatorOpEquals(a, b));
			break;
		}
		default:
			return false;
	}
	return true;
}

TEST_CASE("test redirection node", "[.][parser]") {
	string line = "/bin/ls 5> filename";

	auto input = line + '\n';
	auto chars = LineCharProvider(input);
	auto lexer = Lexer(chars);
	Parser parser(lexer);
	auto result = parser.parse();

	// Create expected result
	auto redir = []() {
		auto redirection = Ast::Redirection();
		redirection.file_name = "filename";
		redirection.redirection_type = Token::Type::Great;
		redirection.io_number = 5;
		return redirection;
	};
	auto lst = [&]() {
		auto list = Ast::List();
		list.append(redir());
		return list;
	};
	auto expected = Ast::Node(Ast::Command("/bin/ls", lst()));

	REQUIRE(Equals(result.root.get(), &expected));
}
