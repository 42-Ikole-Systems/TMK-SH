#pragma once

#include <exception>
#include "shell/utility/types.hpp"

namespace shell {

class NotImplementedException : public std::exception {
public:
	const char *what() const noexcept override;
};

} // namespace shell