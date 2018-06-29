cmake_minimum_required(VERSION 3.1.2)
project(mgs_meta)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(mgs_meta INTERFACE)
target_include_directories(mgs_meta INTERFACE include)

install(TARGETS mgs_meta
  EXPORT mgs_meta
)

install(DIRECTORY include DESTINATION .)
