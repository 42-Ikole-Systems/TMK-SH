#pragma once

#include "util.hpp"
#include "interfaces/provider.hpp"

namespace shell {

class LineCharProvider : public Provider<char> {
private:
	string line;
	size_t index;

public:
	LineCharProvider(const string &line);
	char peek(size_t n = 0) override;
	char consume(size_t n = 0) override;
};

} // namespace shell