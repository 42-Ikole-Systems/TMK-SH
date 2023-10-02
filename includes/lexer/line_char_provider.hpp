#pragma once

#include "lexer/lexer.hpp"
#include "util.hpp"

namespace shell {

class LineCharProvider: public CharProvider {
private:
    string line;
    size_t index;
public:
    LineCharProvider(const string& line);
    char peek() override;
    char consume() override;
};

}