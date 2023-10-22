#pragma once

namespace shell {

inline bool isNewline(char ch) {
	return ch == '\n';
}

inline bool isBackslash(char ch) {
	return ch == '\\';
}

inline bool isSingleQuote(char ch) {
	return ch == '\'';
}

inline bool isDoubleQuote(char ch) {
	return ch == '\"';
}

inline bool isDollarSign(char ch) {
	return ch == '$';
}

inline bool isBackQuote(char ch) {
	return ch == '`';
}

} // namespace shell
