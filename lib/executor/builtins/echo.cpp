
#include "shell/executor/builtins.hpp"

#include <iostream>

namespace shell
{
	ResultCode Builtin::echo(const vector<string>& args) {
		size_t i = 1;
		bool new_line_at_end = true;
		if (args[i].find("-n") == 0) {
			new_line_at_end = false;
			i++;
		}
		while (i < args.size()) {
			std::cout << args[i];
			i++;
			if (i < args.size()) {
				std::cout << ' ';
			}
		}
		if (new_line_at_end) {
			std::cout << std::endl;
		}
		return ResultCode::Ok;
	}
}