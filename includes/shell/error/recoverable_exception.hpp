#pragma once

#include <exception>

namespace shell {

class RecoverableException : public std::exception {};

} // namespace shell
