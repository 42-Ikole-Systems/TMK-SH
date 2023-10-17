add_library(
    test_parser
    OBJECT
    test_io_number.cpp
)

# Forward the declared variable to the parent scope
set(TEST_OBJECT_FILES
    ${TEST_OBJECT_FILES} $<TARGET_OBJECTS:test_parser> PARENT_SCOPE)
