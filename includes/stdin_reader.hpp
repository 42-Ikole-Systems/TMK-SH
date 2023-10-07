#pragma once

#include "reader.hpp"
#include "util.hpp"

namespace shell {

class StdinReader : public Reader {
private:
	string prompt;
	optional<string> line;
	size_t index;

public:
	StdinReader(const string &prompt);
	virtual ~StdinReader();

	optional<string> nextLine() override;
	char nextChar() override;

private:
};

} // namespace shell
