add_library(
    lib_utility
    OBJECT
    split.cpp
)

# Forward the declared variable to the parent scope
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:lib_utility>
    PARENT_SCOPE)
