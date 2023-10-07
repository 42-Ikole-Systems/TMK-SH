# Define minimum version of CMake
cmake_minimum_required(VERSION 3.13.1)

# Set project name
project(app)

# Set C/C++ standards
set(CMAKE_CXX_STANDARD 17)

# set(CMAKE_VERBOSE_MAKEFILE on)

include_directories(${CMAKE_SOURCE_DIR}/includes)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS_DEBUG
      "${CMAKE_CXX_FLAGS_DEBUG} -g3 -Wall -Wextra -Werror -fsanitize=address -DDEBUG"
  )
  set(CMAKE_CXX_FLAGS_DEBUG
      "${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused -Wno-unused-parameter")
endif()

add_subdirectory(src)
add_subdirectory(lib)

# Location of executable source file. Target must be named "app."
add_executable(app ${ALL_OBJECT_FILES})

target_link_libraries(app PRIVATE readline)
