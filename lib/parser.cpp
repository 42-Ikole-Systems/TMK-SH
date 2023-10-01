#include "parser.hpp"
#include <stdexcept>
#include <utility>

namespace shell {

/*

; -> Sequence
    <left>
    <right>
*/

Parser::AST Parser::getNextCommand(vector<Token> tokens) {
    return AST();
}

}
