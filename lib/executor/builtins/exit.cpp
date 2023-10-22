
#include "shell/executor/builtins.hpp"

namespace shell {

[[noreturn]] void Builtin::exit(const vector<string> &args) {
	if (args.empty()) {
		exit(ResultCode::Ok);
	}
	::exit(std::stoi(args[0]));
}

[[noreturn]] void Builtin::exit(ResultCode code) {
	::exit(static_cast<int>(code));
}

} // namespace shell
