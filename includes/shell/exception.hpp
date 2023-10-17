#pragma once

#include <exception>
#include "shell/util.hpp"
#include <string>

namespace shell {

class NotImplementedException : public std::exception {
public:
	NotImplementedException(const std::string &reason = "") {
		std::string message = "Not Implemented Error";
		if (!reason.empty()) {
			message += ": " + reason;
		}
		this->msg = message;
	}

public:
	const char *what() const noexcept override;

public:
	std::string msg;
};

} // namespace shell