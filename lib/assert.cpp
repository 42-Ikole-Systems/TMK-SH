#include "shell/assert.hpp"
#include <errno.h>
#include <cstdio>
#include <string.h>

namespace shell {

void AssertInternal(bool condition, const char *condition_name, const char *file, int linenr) {
	// todo: print stack trace
	if (condition) {
		return;
	}
	LOG_ERROR("Assertion triggered in file \"%\" on line %: %\n", file, linenr, condition_name);
	abort();
}

void SyscallError(const char *s, const char *file, int linenr) {
#ifdef DEBUG
	LOG_ERROR("syscall error: %: % [%:%]\n", s, strerror(errno), file, linenr);
#else
	LOG_ERROR("syscall error: %: %\n", s, strerror(errno));
#endif
}

} // namespace shell
