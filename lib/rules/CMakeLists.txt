add_library(
    lib_rules
    OBJECT
    and_or.cpp
    list_rule.cpp
    separator_op.cpp
)

# Forward the declared variable to the parent scope
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:lib_rules>
    PARENT_SCOPE)
