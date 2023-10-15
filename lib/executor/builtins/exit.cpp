
#include "shell/executor/builtins.hpp"

namespace shell
{

[[noreturn]] void Builtin::exit(const vector<string>& args) {
	if (args.size() == 1) {
		exit(ResultCode::Ok);
	}
	::exit(std::stoi(args[1]));
}

[[noreturn]] void Builtin::exit(ResultCode code) {
	::exit(static_cast<int>(code));
}

}
