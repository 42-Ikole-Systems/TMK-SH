#include "shell/grammar/list.hpp"
#include "shell/assert.hpp"
#include "shell/grammar/rule.hpp"

namespace shell {

Rule::grammar_func_t ListConstructor::ListCreator(std::function<Ast::Node(Ast::Node)> func) {
	auto grammar_func = [=](vector<Ast::Node> &args) -> optional<Ast::Node> {
		D_ASSERT(args.size() == 1);
		// This is stupid, but couldn't find a better alternative

		// Move all items to a temporary container
		vector<Ast::Node> tmp(std::make_move_iterator(args.begin()), std::make_move_iterator(args.end()));

		args.clear();

		// Apply the provided function to the item
		args.push_back(func(std::move(tmp[0])));
		return CreateList(args);
	};
	return grammar_func;
}

Rule::grammar_func_t ListConstructor::ListAppender(std::function<Ast::Node(Ast::Node)> func) {
	auto grammar_func = [=](vector<Ast::Node> &args) -> optional<Ast::Node> {
		D_ASSERT(args.size() == 2);
		// Move all items to a temporary container
		vector<Ast::Node> tmp(std::make_move_iterator(args.begin()), std::make_move_iterator(args.end()));

		args.clear();

		// Apply the provided function to the item
		args.push_back(func(std::move(tmp[0])));
		args.push_back(std::move(tmp[1]));
		return AppendToList(args);
	};
	return grammar_func;
}

optional<Ast::Node> ListConstructor::CreateList(vector<Ast::Node> &node) {
	D_ASSERT(node.size() == 1);
	return Ast::List(std::move(node));
}

optional<Ast::Node> ListConstructor::AppendToList(vector<Ast::Node> &args) {
	D_ASSERT(args.size() == 2);
	auto &list_arg = args[1].get<Ast::List>();

	list_arg.append(std::move(args[0]));
	LOG_DEBUG("CommandPrefix: Append to list\n");
	return std::move(list_arg);
}

} // namespace shell
