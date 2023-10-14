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
	// TODO: implement
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

optional<Ast::Node> IOFile::handler(vector<Ast::Node> &args) {
	D_ASSERT(args.size() == 3);
	auto redir = Ast::Redirection();
	redir.left = make_unique<Ast::Node>(std::move(args[0]));
	redir.right = make_unique<Ast::Node>(std::move(args[1]));

	auto &literal = args[1].get<Ast::Literal>();
	auto &token = literal.token;
	D_ASSERT(isRedirection(token.getType()));

	redir.redirection_type = token.getType();
	return redir;
}

} // namespace shell
