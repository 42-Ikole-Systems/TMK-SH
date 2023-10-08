add_library(
    lib
    OBJECT
    lexer.cpp
    parser.cpp
    stdin_reader.cpp
    token.cpp
    line_char_provider.cpp
    ast.cpp
    assert.cpp
    environment.cpp
)

add_subdirectory(grammar)
add_subdirectory(executor)

# Forward the declared variable to the parent scope
set(ALL_OBJECT_FILES ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:lib>)
set(ALL_OBJECT_FILES ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:lib> PARENT_SCOPE)

add_library(shell ${ALL_OBJECT_FILES})
