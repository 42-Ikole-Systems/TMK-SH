#pragma once

#include "shell/util.hpp"
#include "shell/interfaces/provider.hpp"

namespace shell {

class LineCharProvider : public Provider<char> {
private:
	string line;
	size_t index;

public:
	LineCharProvider(const string &line);
	char peek() override;
	char consume() override;

	void remove() override;
	void unconsume() override;
};

} // namespace shell