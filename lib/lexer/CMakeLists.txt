add_library(
    lib_lexer
    OBJECT
    lexer.cpp
    line_char_provider.cpp
    reader_char_provider.cpp
    token.cpp
)

# Forward the declared variable to the parent scope
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:lib_lexer>
    PARENT_SCOPE)
