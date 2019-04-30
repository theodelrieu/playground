cmake_minimum_required(VERSION 3.3)

project(test_helpers VERSION 0.1.0 LANGUAGES CXX)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS NO_OUTPUT_DIRS)

add_library(catch_main OBJECT src/catch_main.cpp)
get_target_property(CATCH2_INC_DIR CONAN_PKG::Catch2 INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(catch_main PRIVATE ${CATCH2_INC_DIR})

add_library(mgs_test_helpers INTERFACE)
target_include_directories(mgs_test_helpers INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

install(TARGETS catch_main
  EXPORT catch_main
  EXCLUDE_FROM_ALL
  OBJECTS
    DESTINATION bin
)

install(DIRECTORY include DESTINATION . EXCLUDE_FROM_ALL)
