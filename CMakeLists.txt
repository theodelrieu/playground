cmake_minimum_required(VERSION 3.1.2)

project(mgs)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(mgs INTERFACE)

target_include_directories(mgs INTERFACE include)
target_link_libraries(mgs INTERFACE CONAN_PKG::optional-lite)

include(CTest)

if (BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()
