add_library(
    lib_executor
    OBJECT
    command.cpp
    executor.cpp
    separator_op.cpp
)

# Forward the declared variable to the parent scope
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:lib_executor>
    PARENT_SCOPE)
