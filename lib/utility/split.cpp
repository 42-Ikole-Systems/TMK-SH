
#include "shell/utility/split.hpp"

namespace shell
{
	LazySplit::LazySplit(const string& source_, const string& delimiter_)
		: source(source_)
		, delimiter(delimiter_)
	{

	}

	pair<size_t, std::string_view> LazySplit::next(size_t pos)
	{
		if (pos >= source.length()) {
			return {string::npos, nullptr};
		}
		const auto delim_pos = source.find(delimiter, pos); 
		return {delim_pos + delimiter.length(), std::string_view(source.data() + pos, delim_pos)};
	}

	LazySplit::iterator LazySplit::begin() {
		auto [pos, view] = next(0);
		return iterator(view, *this, pos);
	}

	LazySplit::iterator LazySplit::end() {
		return iterator(std::string_view(source.data(), source.length()), *this, string::npos);
	}

	ForwardLazySpliterator::ForwardLazySpliterator(std::string_view view_, LazySplit& splitter_, size_t pos_)
	: view(view_)
	, splitter(splitter_)
	, pos(pos_)
	{

	}

	std::string_view& ForwardLazySpliterator::operator * () {
		return view;
	}

	ForwardLazySpliterator& ForwardLazySpliterator::operator ++ () {
		auto [delim_pos, new_view] = splitter.next(pos);
		pos = delim_pos;
		view = new_view;
		return *this;
	}

	ForwardLazySpliterator ForwardLazySpliterator::operator ++ (int) {
		auto old = ForwardLazySpliterator(view, splitter, pos);
		++(*this);
		return old;
	}

	bool ForwardLazySpliterator::operator != (const ForwardLazySpliterator& x) {
		return (this->pos != x.pos);
	}

}
