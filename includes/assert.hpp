#pragma once

#include <stdlib.h>
#include "logger.hpp"

#define D_ASSERT(condition) shell::AssertInternal(bool(condition), #condition, __FILE__, __LINE__)
#define SYSCALL_ERROR(s)    shell::SyscallError(s, __FILE__, __LINE__)

namespace shell {

void AssertInternal(bool condition, const char *condition_name, const char *file, int linenr);
void SyscallError(const char *s, const char *file, int linenr);

} // namespace shell