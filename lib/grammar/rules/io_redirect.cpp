#include "shell/grammar/rules/io_redirect.hpp"
#include "shell/grammar/grammar_util.hpp"
#include "shell/grammar/rules/io_file.hpp"
#include "shell/grammar/rules/io_here.hpp"
#include "shell/assert.hpp"

namespace shell {

Rule IORedirect::make() {
	return Rule {
	    IORedirect::options,
	};
}

vector<Rule::Option> IORedirect::options() {
	auto placeholder = [](vector<Ast::Node> &args) -> optional<Ast::Node> {
		return nullopt;
	};
	auto handler = [](vector<Ast::Node> &args) -> optional<Ast::Node> {
		if (args.size() == 2) {
			auto &io_number_token = args[0].get<Ast::Literal>().token.get<IoNumber>();
			auto io_number = std::stoul(io_number_token.value);

			auto &redirect_node = args[1].get<Ast::Redirection>();
			redirect_node.io_number = io_number;
			return std::move(redirect_node);
		} else {
			D_ASSERT(args.size() == 1);
			auto &redirect_node = args[0].get<Ast::Redirection>();
			return std::move(redirect_node);
		}
	};
	return {Rule::NonTerminal(handler, {GrammarUtil::ConsumeIf<Token::Type::IoNumber>::make(), IOFile::make()}),
	        Rule::NonTerminal(placeholder, {GrammarUtil::ConsumeIf<Token::Type::IoNumber>::make(), IOHere::make()}),
	        Rule::NonTerminal(handler, {IOFile::make()}), Rule::NonTerminal(placeholder, {IOHere::make()})};
}

} // namespace shell
