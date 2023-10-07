#include "grammar/rules/and_or.hpp"
#include "interfaces/provider.hpp"

namespace shell {

Rule AndOr::make() {
	return Rule {
	    AndOr::options,
	};
}

vector<Rule::Option> AndOr::options() {
	return {Rule::Terminal {handler}};
}

optional<Ast::Node> AndOr::handler(Provider<Token> &tokens) {
	vector<string> words;
	while (true) {
		auto token = tokens.peek();
		// if (!token.has_value()) {
		//	break;
		//}
		if (token.type != Token::Type::Word) {
			break;
		}
		words.emplace_back(token.word_token.value);
		tokens.consume();
	}
	if (words.empty()) {
		return nullopt;
	}
	return Ast::Command(std::move(words));
}

} // namespace shell
