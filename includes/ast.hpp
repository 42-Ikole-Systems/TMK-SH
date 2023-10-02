#pragma once

#include "util.hpp"

namespace shell {

class Ast {
public:
    class Node {
        public:
            enum class Type { SeparatorOp, Command };
            virtual ~Node() {}
            virtual Type getType() const = 0;
            virtual void print(int level) const = 0;
    };

    struct Command: public Node {
        Command(vector<string>&& args);
        ~Command();

        Type getType() const override;
        void print(int level) const override;

        vector<string> args; // args[0] == program name
    };

    struct SeparatorOp: public Node {
        ~SeparatorOp();

        Type getType() const override;
        void print(int level) const override;
        // ; &
        unique_ptr<Node> left;
        unique_ptr<Node> right;
    };

public:
    Ast(unique_ptr<Node> root);

    unique_ptr<Node> root;

    void print() const;
};

}