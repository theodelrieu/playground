cmake_minimum_required(VERSION 3.1.2)
project(mgs_adapters)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_library(mgs_adapters INTERFACE)
target_include_directories(mgs_adapters INTERFACE include)
target_link_libraries(mgs_adapters INTERFACE CONAN_PKG::mgs_meta CONAN_PKG::mgs_iterators CONAN_PKG::mgs_adapters_base)


install(TARGETS mgs_adapters
  EXPORT mgs_adapters
)

install(DIRECTORY include DESTINATION .)
