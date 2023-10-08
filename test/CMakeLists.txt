include_directories(${CMAKE_SOURCE_DIR}/includes/third_party/catch)
include_directories(${CMAKE_SOURCE_DIR}/test/includes)

add_library(
    tests
    OBJECT
    test.cpp
	test_helpers.cpp
)

add_subdirectory(lexer)

# Forward the declared variable to the parent scope
set(TEST_OBJECT_FILES
    ${TEST_OBJECT_FILES} $<TARGET_OBJECTS:tests>)

# Add the test executable
add_executable(test ${TEST_OBJECT_FILES})

target_link_libraries(test PRIVATE shell)

## Add a custom target to run tests
#add_custom_target(run_tests COMMAND ./test DEPENDS test)
