#pragma once

#include "shell/error/recoverable_exception.hpp"
#include "shell/util.hpp"

namespace shell {

class SyntaxErrorException : public RecoverableException {
private:
	const string reason;
public:
	SyntaxErrorException(const string& reason);
	const char *what() const noexcept override;
};

}