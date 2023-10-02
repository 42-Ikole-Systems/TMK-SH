add_library(
    lib
    OBJECT
    lexer.cpp
    parser.cpp
    stdin_reader.cpp
    token.cpp
    line_char_provider.cpp
)

# Forward the declared variable to the parent scope
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:lib>
    PARENT_SCOPE)
