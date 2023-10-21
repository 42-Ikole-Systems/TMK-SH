#include "shell/exception.hpp"

namespace shell {

const char *NotImplementedException::what() const noexcept {
	return msg.c_str();
}

} // namespace shell
