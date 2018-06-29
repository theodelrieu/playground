cmake_minimum_required(VERSION 3.1.2)
project(mgs_base_n)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(mgs_base_n INTERFACE)
target_include_directories(mgs_base_n INTERFACE include)

install(TARGETS mgs_base_n
  EXPORT mgs_base_n
)

install(DIRECTORY include DESTINATION .)
