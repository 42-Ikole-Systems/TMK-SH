#pragma once

#include "reader.hpp"
#include "util.hpp"

namespace shell {

class StdinReader : public Reader {
private:
	string prompt;

public:
	StdinReader(const string &prompt);
	virtual ~StdinReader();

	optional<string> nextLine() override;
};

} // namespace shell
