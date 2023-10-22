add_library(
    lib_environment
    OBJECT
    global_environment.cpp
	shell_environment.cpp
	singleton_shell_environment.cpp
)

# Forward the declared variable to the parent scope
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:lib_environment>
    PARENT_SCOPE)
