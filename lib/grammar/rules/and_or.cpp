#include "shell/grammar/rules/and_or.hpp"
#include "shell/interfaces/provider.hpp"

namespace shell {

Rule AndOr::make() {
	return Rule {
	    AndOr::options,
	};
}

vector<Rule::Option> AndOr::options() {
	return {Rule::Terminal {handler}};
}

optional<Ast::Node> AndOr::handler(TokenProvider &tokens) {
	vector<string> words;
	while (true) {
		auto token = tokens.peek();
		if (!token.has_value()) {
			break;
		}
		if (token->getType() != Token::Type::Word) {
			break;
		}
		auto &word_token = token->get<WordToken>();
		words.emplace_back(word_token.value);
		tokens.consume();
	}
	if (words.empty()) {
		return nullopt;
	}
	return Ast::Command(std::move(words));
}

} // namespace shell
