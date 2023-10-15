add_library(
    lib_builtin
    OBJECT
    echo.cpp
	exit.cpp
)

# Forward the declared variable to the parent scope
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:lib_builtin>
    PARENT_SCOPE)