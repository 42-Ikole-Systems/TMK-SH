#pragma once

#include <cstddef>

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
};
