cmake_minimum_required(VERSION 3.1.2)
project(mgs_adapters_base)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_library(mgs_adapters_base INTERFACE)
target_include_directories(mgs_adapters_base INTERFACE include)
target_link_libraries(mgs_adapters_base INTERFACE CONAN_PKG::mgs_meta)


install(TARGETS mgs_adapters_base
  EXPORT mgs_adapters_base
)

install(DIRECTORY include DESTINATION .)
