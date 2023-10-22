add_library(
    lib_expansion
    OBJECT
    quote_removal.cpp
)

# Forward the declared variable to the parent scope
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:lib_expansion>
    PARENT_SCOPE)
