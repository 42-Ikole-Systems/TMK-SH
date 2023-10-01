#include "parser.hpp"
#include <stdexcept>
#include <utility>

namespace shell {

Parser::Parser(Lexer &&lexer): lexer(std::move(lexer)) {}

Parser::AST Parser::getNextCommand() {
    throw std::runtime_error("not implemented");
}

}
