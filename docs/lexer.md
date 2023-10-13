# Design

- State machine based lexer

## Additional reading

Concept: Lexer receivs a CharProvider, which holds a Reader and will lazily read from STDIN.  
This allows the lexer to ignore or consume newlines without producing a newline tokens, namely in any state other than the EMPTY state.

### Here-doc [ draft, not implemented ]

Idea: When a here-doc is encountered, the lexer will stop consuming input and will read manually using a separate reader received as parameter.

## Recursive lexing (i.e. expansions)

Some states expect to return to the previous state:
- Backslash
- CommandSubstitution
- DoubleQuotes
- SingleQuotes
- ParameterExpansion
- ArithmeticExpansion

This means the precondition to enter these states, is to push the current state onto the stack, and enter those states.

For example, for Word state entering DoubleQuote:

```C++
if (isDoubleQuote(ch)) {
	pushState(State::Word);
	return State::DoubleQuoteStart;
}
```

Then, at the end of the target state, we can pop the previous state off the stack:

```C++
State doubleQuoteState() {
	char ch = chars.consume();
	if (isDoubleQuote(ch)) {
		state_data.words.push_back(ch);
		return popState(); // pop previous state off the stack
	}
}
```

# To do

- Expansions and recursive lexing implementation
- Here doc lexing implementation (assuming some metadata should be saved for the expander)
- Determine behavior when encountering EOF in some unexpected states (such as backslash, quotes, anything that expects a closing character)

# Improvements

Currently one or a few characters are processed per state.
Ideally, a state is ran until it is either complete or has to switch state. Similar to how green thread execution works.