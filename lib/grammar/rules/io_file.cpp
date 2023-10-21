#include "shell/grammar/rules/io_file.hpp"
#include "shell/grammar/rules/filename.hpp"
#include "shell/grammar/grammar_util.hpp"
#include "shell/assert.hpp"

namespace shell {

Rule IOFile::make() {
	return Rule {
	    IOFile::options,
	};
}

vector<Rule::Option> IOFile::options() {
	return {CreateOption<Token::Type::Less>(),        CreateOption<Token::Type::LessAnd>(),
	        CreateOption<Token::Type::Great>(),       CreateOption<Token::Type::GreatAnd>(),
	        CreateOption<Token::Type::DoubleGreat>(), CreateOption<Token::Type::LessGreat>(),
	        CreateOption<Token::Type::Clobber>()};
}

static bool isRedirection(Token::Type type) {
	switch (type) {
		case Token::Type::Less:
		case Token::Type::LessAnd:
		case Token::Type::Great:
		case Token::Type::GreatAnd:
		case Token::Type::DoubleGreat:
		case Token::Type::LessGreat:
		case Token::Type::Clobber:
			return true;
		default:
			return false;
	}
}

size_t getDefaultIONumber(Token::Type type) {
	static constexpr size_t STDIN = 0;
	static constexpr size_t STDOUT = 1;

	switch (type) {
		case Token::Type::Less:
			return STDIN;
		case Token::Type::Great:
		case Token::Type::Clobber:
			return STDOUT;
		case Token::Type::DoubleGreat:
			return STDOUT;
		case Token::Type::LessAnd:
			return STDIN;
		case Token::Type::GreatAnd:
			return STDOUT;
		case Token::Type::LessGreat:
			return STDIN;
		default:
			throw std::runtime_error("Invalid redirection type encountered!");
	}
}

optional<Ast::Node> IOFile::handler(vector<Ast::Node> &args) {
	D_ASSERT(args.size() == 2);
	auto redir = Ast::Redirection();

	auto &file_name = args[1].get<Ast::Literal>();
	redir.file_name = file_name.token.get<WordToken>().value;

	auto &literal = args[0].get<Ast::Literal>();
	auto &token = literal.token;
	D_ASSERT(isRedirection(token.getType()));
	redir.io_number = getDefaultIONumber(token.getType());

	redir.redirection_type = token.getType();
	return redir;
}

} // namespace shell
