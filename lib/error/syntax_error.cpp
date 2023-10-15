#include "shell/error/syntax_error.hpp"

namespace shell {

SyntaxErrorException::SyntaxErrorException(const string& reason): reason(reason) {}

const char *SyntaxErrorException::what() const noexcept {
	return reason.c_str();
}

}
