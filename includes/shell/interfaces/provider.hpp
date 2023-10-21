#pragma once

#include <cstddef>
#include "shell/exception.hpp"

namespace shell {

template <typename T>
class Provider {
public:
	virtual ~Provider() {
	}

	/**
	 * @brief view next item in stream, without removing it
	 *
	 * @return T next item in stream
	 */
	virtual T peek() = 0;

	/**
	 * @brief remove T from stream
	 *
	 * @return T that was removed
	 */
	virtual T consume() = 0;

	/**
	 * @brief Remove token from the stream instead of consuming
	 * This is different in for example the special case where there is a backslash followed by a newline.
	 */
	virtual void remove() {
		throw NotImplementedException();
	}
	/**
	 * @brief Undoes the consume() operation
	 */
	virtual void unconsume() {
		throw NotImplementedException();
	}
};

} // namespace shell
