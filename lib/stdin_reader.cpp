#include "stdin_reader.hpp"
#include <readline/readline.h>
#include <stdlib.h>
#include "util.hpp"

namespace shell {

StdinReader::StdinReader(const string &prompt): prompt(prompt) {}

StdinReader::~StdinReader() {}

optional<string> StdinReader::nextLine() {
    char* line = readline(prompt.c_str());
    if (line == nullptr) {
        return nullopt;
    }
    // todo: optimize string copying
    string result = string(line);
    free(line);
    return result;
}

} //namespace shell
