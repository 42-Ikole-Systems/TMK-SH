#include "shell/interfaces/provider.hpp"
#include "shell/interfaces/reader.hpp"

namespace shell {

class ReaderCharProvider : public Provider<char> {
	Reader &reader;
	string line;
	size_t index;
	bool eof;

public:
	ReaderCharProvider(Reader &reader);

	char peek() override;
	char consume() override;
	void remove() override;
	void unconsume() override;

	/**
	 * @brief Moves line out of provider, resetting the state
	 *
	 * @return string&& moved line
	 */
	string takeLine();
	bool isEof() const;

private:
	bool ensureChar();
	void reset();
};

} // namespace shell