#pragma once

#include "util.hpp"
#include "token.hpp"

namespace shell {

class TokenParser {
public:
    enum class Result {
        // meaning nextState() will be called without a call to Delimit
        StateTransition,
        Continue,
        // delimit() -> nextState()
        Delimit,
    };

    virtual ~TokenParser() {}
    virtual Result process(char ch) = 0;
    virtual unique_ptr<TokenParser> nextState() = 0;
    virtual Token delimit() = 0;
};

}
