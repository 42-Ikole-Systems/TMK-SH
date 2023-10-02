#pragma once

#include <util.hpp>

namespace shell {

/*!
 * @brief interface for Readers.
 */
class Reader {
public:
	virtual ~Reader() {};
	/*!
	 * @brief Gets line from source.
	 * @return line read, nullopt if EOF is reached.
	 */
	virtual optional<string> nextLine() = 0;

	/**
	 * @brief Gets char from source
	 *
	 * @return char 0 on EOF
	 */
	virtual char nextChar() = 0;
};

} // namespace shell
