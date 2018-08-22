cmake_minimum_required(VERSION 3.1.2)
project(mgs_binary_to_text)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_library(mgs_binary_to_text INTERFACE)
target_include_directories(mgs_binary_to_text INTERFACE include)
target_link_libraries(mgs_binary_to_text INTERFACE CONAN_PKG::mgs_meta CONAN_PKG::mgs_exceptions)

include(CTest)

if (BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

install(TARGETS mgs_binary_to_text
  EXPORT mgs_binary_to_text
)

install(DIRECTORY include DESTINATION .)
