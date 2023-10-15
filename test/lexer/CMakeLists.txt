add_library(
    test_lexer
    OBJECT
    basic_ls.cpp
    stdin_lexer.cpp
)

# Forward the declared variable to the parent scope
set(TEST_OBJECT_FILES
    ${TEST_OBJECT_FILES} $<TARGET_OBJECTS:test_lexer> PARENT_SCOPE)
