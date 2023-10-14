#pragma once

#include <iostream>
#include "shell/util.hpp"

namespace shell {

static void tprintf(const char *format) // base function
{
	std::cout << format;
}

static void tprintf(const string &format) {
	std::cout << format;
}

template <typename T, typename... Targs>
void tprintf(const char *format, T value, Targs... Fargs) // recursive variadic function
{
	for (; *format != '\0'; format++) {
		if (*format == '%') {
			std::cout << value;
			tprintf(format + 1, Fargs...); // recursive call
			return;
		}
		std::cout << *format;
	}
}

} // namespace shell
