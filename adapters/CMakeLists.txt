cmake_minimum_required(VERSION 3.1.2)
project(mgs_adapters)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(mgs_adapters INTERFACE)
target_include_directories(mgs_adapters INTERFACE include)

install(TARGETS mgs_adapters
  EXPORT mgs_adapters
)

install(DIRECTORY include DESTINATION .)
