#include "lexer.hpp"
#include <stdexcept>
#include "util.hpp"

namespace shell {

Lexer::Lexer(unique_ptr<Reader> reader) : reader(std::move(reader)) {}

Lexer::Lexer(Lexer&& other): reader(std::move(other.reader)) {}

Lexer::Token Lexer::getNextToken() {
    throw std::runtime_error("not implemented");
}

}; //namespace shell
