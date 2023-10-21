add_library(
    lib_error
    OBJECT
    syntax_error.cpp
)

# Forward the declared variable to the parent scope
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:lib_error>
    PARENT_SCOPE)
