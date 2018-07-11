cmake_minimum_required(VERSION 3.1.2)
project(mgs_test_helpers)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_library(mgs_test_helpers INTERFACE)
target_include_directories(mgs_test_helpers INTERFACE include)

add_library(catch_main OBJECT src/catch_main.cpp)
get_target_property(CATCH2_INC_DIR CONAN_PKG::catch2 INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(catch_main PRIVATE ${CATCH2_INC_DIR})
