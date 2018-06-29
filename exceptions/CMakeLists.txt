cmake_minimum_required(VERSION 3.1.2)
project(mgs_exceptions)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(mgs_exceptions INTERFACE)
target_include_directories(mgs_exceptions INTERFACE include)

install(TARGETS mgs_exceptions
  EXPORT mgs_exceptions
)

install(DIRECTORY include DESTINATION .)
