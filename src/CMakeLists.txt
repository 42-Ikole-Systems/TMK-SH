add_library(
    src
    OBJECT
    main.cpp
)

# Forward the declared variable to the parent scope
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:src>
    PARENT_SCOPE)

