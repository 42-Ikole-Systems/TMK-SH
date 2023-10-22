add_library(
    lib_grammar
    OBJECT
    rule.cpp
	list.cpp
)

add_subdirectory(rules)

# Forward the declared variable to the parent scope
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:lib_grammar>
    PARENT_SCOPE)
