add_library(
    lib_lexer
    OBJECT
    double_quote_state_handler.cpp
)

# Forward the declared variable to the parent scope
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:lib_lexer>
    PARENT_SCOPE)
