add_library(
    lib
    OBJECT
    parser.cpp
    stdin_reader.cpp
    ast.cpp
    assert.cpp
    exception.cpp
)

add_subdirectory(lexer)
add_subdirectory(grammar)
add_subdirectory(executor)

# Forward the declared variable to the parent scope
set(ALL_OBJECT_FILES ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:lib>)
set(ALL_OBJECT_FILES ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:lib> PARENT_SCOPE)

add_library(shell ${ALL_OBJECT_FILES})
