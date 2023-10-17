#include "shell/lexer/token_provider.hpp"
#include "shell/logger.hpp"

namespace shell {

optional<Token> TokenProvider::consumeIf(consume_predicate_t predicate) {
	auto token = peek();
	if (!token.has_value()) {
		return nullopt;
	}
	if (predicate(token.value())) {
		consume();
		return token;
	}
	return nullopt;
}

} // namespace shell
