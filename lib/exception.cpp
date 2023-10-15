#include "shell/exception.hpp"

namespace shell {

const char *NotImplementedException::what() const noexcept {
	return "not implemented";
}

} // namespace shell
