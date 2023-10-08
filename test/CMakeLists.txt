include_directories(${CMAKE_SOURCE_DIR}/includes/third_party/catch)


# Add the test executable
add_executable(test test.cpp)

target_link_libraries(test PRIVATE shell)

## Add a custom target to run tests
#add_custom_target(run_tests COMMAND ./test DEPENDS test)
