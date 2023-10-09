
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
			return {string::npos, {}};
		}
		const auto delim_pos = source.find(delimiter, pos);
		// Last occurrence.
		if (delim_pos == string::npos) {
			return {string::npos, std::string_view(source.data() + pos, source.length() - pos)};
		}
		return {delim_pos + delimiter.length(), std::string_view(source.data() + pos, delim_pos - pos)};
	}

	LazySplit::iterator LazySplit::begin() {
		auto [pos, view] = next(0);
		return iterator(view, *this, pos);
	}

	LazySplit::iterator LazySplit::end() {
		return iterator({}, *this, string::npos);
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
		const auto& [delim_pos, new_view] = splitter.next(pos);
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
		return (this->pos != x.pos && this->view != x.view);
	}

}
