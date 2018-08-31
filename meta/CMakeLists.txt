cmake_minimum_required(VERSION 3.1.2)
project(mgs_meta)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_library(mgs_meta INTERFACE)
target_include_directories(mgs_meta INTERFACE include)

install(TARGETS mgs_meta
  EXPORT mgs_meta
)

install(DIRECTORY include DESTINATION .)

if (BUILD_TESTING)
  add_subdirectory(test)
endif()
