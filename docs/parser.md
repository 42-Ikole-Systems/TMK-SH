# Design

- Rule based recursive descent parsing

## Additional reading

Concept: Parser receives a TokenProvider, each NonTerminal rule dictates what it requires, when we reach a Terminal rule we conditionally consume a Token, if the parsing of a branch fails we backtrack and check the next valid branch.  

## Rules

A Rule is a Node-producing function.  
These come in two flavors;  
- Terminal - Consumes a Token of a specific type  
- NonTerminal - Consists of one or more Options  

### Option

This is simply a list of rules that make up a recipe, once all elements of the recipe have produced a Node, the callback of the option is invoked which gets fed all the Nodes that the recipe produced.

### Greedy Matching

NonTerminal rules are greedy in nature, meaning that the first Option that matches gets used, so we have to be careful in the way we order the Options for a given NonTerminal rule.

### Recursive Rules

NonTerminals can also be recursive, meaning one (or more) of the Options can contain the Rule we're currently processing.

To make sure this works, the recursive Rule always has to be the last element in the Option.

## Node

Nodes come in many different types, some general purpose nodes;
- Literal - this Node is often the building block of other Nodes, often produced by Terminals because they only consist of a single Token.
- List - Contains a list of Nodes, often used in recursive Rules

Many more node types exist and are used within the Executor.
