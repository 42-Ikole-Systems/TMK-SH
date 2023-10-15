
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
	const string source;
	const string delimiter;

	using iterator = ForwardLazySpliterator;

public:
	/*!
	 * @brief
	 * @param source String you want to split up.
	 * @param delimiter Consecutive character(s) you want to split on.
	 */
	LazySplit(const string &source, const string &delimiter);

	/*!
	 * @brief Gets next substring by delimiter.
	 * @param pos Starts looking from here.
	 * @return pair<delimiter_pos, substring>
	 */
	pair<size_t, std::string_view> next(size_t pos);

	ForwardLazySpliterator begin();
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

	/*!
	 * @brief
	 * @param view Of the substring that was split on.
	 * @param splitter Reference to LazySplit instance for getting the next split.
	 * @param pos Pos after delimiter last found.
	*/
	ForwardLazySpliterator(std::string_view view, LazySplit &splitter, size_t pos);

	std::string_view &operator*();
	ForwardLazySpliterator &operator++();
	ForwardLazySpliterator operator++(int);

	bool operator!=(const ForwardLazySpliterator &x);
};

} // namespace shell
