#include "parser.hpp"
#include "print.hpp"
#include <stdexcept>
#include <utility>

namespace shell {

Parser::Parser(Provider<optional<Token>>& tokens): tokens(tokens) {}

/*
a ; b ; c ; d => Sequence (
    Command(a),
    Sequence
        Command(b),
        Sequence(
            Command(c),
            Command(d)
        )
    )
)
Basically: command becomes left side of next sequence, and that sequence becomes the right side of the previous sequence.
Or the command becomes the right side of the previous sequence.
If there is no next or previous sequence, it is simply a command.
If the separatorOp == &, the command becomes `Detach` NodeType, which has Command as a child
*/
Ast Parser::getNextCommand() {
    while (tokens.peek().has_value()) {
        Token token = tokens.consume().value();
        token.print();
    }
    return Ast(nullptr);
}

}
