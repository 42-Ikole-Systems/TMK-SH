#pragma once

#include <cstddef>

template <typename T>
class Provider {
public:
	virtual ~Provider() {
	}

	/**
	 * @brief Returns the n'th token without consuming it
	 * 
	 * @param n Number of tokens to look forward (default: 0 = next token)
	 * @return T
	 */
	virtual T peek(size_t n = 0) = 0;

	/**
	 * @brief Returns the n'th token, consuming up to and including the n'th token
	 * 
	 * @param n Number of T to consume (i.e. ignore) (default: 0 = next token)
	 * 			the last consumed T will be returned
	 * @return T
	 */
	virtual T consume(size_t n = 0) = 0;
};
