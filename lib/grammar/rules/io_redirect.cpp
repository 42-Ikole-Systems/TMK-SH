#include "shell/grammar/rules/io_redirect.hpp"
#include "shell/grammar/grammar_util.hpp"
#include "shell/grammar/rules/io_file.hpp"
#include "shell/grammar/rules/io_here.hpp"

namespace shell {

Rule IORedirect::make() {
	return Rule {
	    IORedirect::options,
	};
}

vector<Rule::Option> IORedirect::options() {
	// TODO: implement
	auto placeholder = [](vector<Ast::Node> &args) -> optional<Ast::Node> {
		return nullopt;
	};
	return {Rule::NonTerminal(placeholder, {GrammarUtil::ConsumeIf<Token::Type::IoNumber>::make(), IOFile::make()}),
	        Rule::NonTerminal(placeholder, {GrammarUtil::ConsumeIf<Token::Type::IoNumber>::make(), IOHere::make()}),
	        Rule::NonTerminal(placeholder, {IOFile::make()}), Rule::NonTerminal(placeholder, {IOHere::make()})};
}

} // namespace shell
