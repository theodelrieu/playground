set(CMAKE_CXX_COMPILER_WORKS 1)
cmake_minimum_required(VERSION 3.3)
project(test_helpers VERSION 0.1 LANGUAGES CXX)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_library(catch_main OBJECT src/catch_main.cpp)
get_target_property(CATCH2_INC_DIR CONAN_PKG::catch2 INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(catch_main PRIVATE ${CATCH2_INC_DIR})

install(TARGETS catch_main
  EXPORT catch_main
  EXCLUDE_FROM_ALL
  OBJECTS
    DESTINATION bin
)
