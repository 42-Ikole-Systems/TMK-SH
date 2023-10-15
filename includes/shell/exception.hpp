#pragma once

#include <exception>
#include "shell/util.hpp"

namespace shell {

class NotImplementedException : public std::exception {
public:
	const char *what() const noexcept override;
};

} // namespace shell