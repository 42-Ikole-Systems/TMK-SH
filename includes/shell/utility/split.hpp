
#pragma once

#include "types.hpp"

#include <string>
#include <string_view>

namespace shell {

class ForwardLazySpliterator; // forward decleration

/*!
 * @brief Lazily split strings.
 */
class LazySplit {
	const string source;    /*!< -.*/
	const string delimiter; /*!< -. */

	using iterator = ForwardLazySpliterator; /*!< -. */

public:
	/*!
	 * @brief .-
	 * @param source
	 * @param delimiter
	 */
	LazySplit(const string &source, const string &delimiter);

	/*!
	 * @brief Gets next substring by delimiter.
	 * @param pos Starts looking from here.
	 * @return pair<delimiter_pos, substring>
	 */
	pair<size_t, std::string_view> next(size_t pos);

	/*!
	 * @brief -.
	 * @return
	 */
	ForwardLazySpliterator begin();

	/*!
	 * @brief -.
	 * @return
	 */
	ForwardLazySpliterator end();
};

/*!
 * @brief Forward iterator for lazyily splitting.
 */
class ForwardLazySpliterator {
	std::string_view view;
	LazySplit &splitter;
	size_t pos;

public:
	ForwardLazySpliterator(std::string_view view, LazySplit &splitter, size_t pos);

	std::string_view &operator*();
	ForwardLazySpliterator &operator++();
	ForwardLazySpliterator operator++(int);

	bool operator!=(const ForwardLazySpliterator &x);
};

} // namespace shell
