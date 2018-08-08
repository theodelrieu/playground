cmake_minimum_required(VERSION 3.1.2)
project(mgs_codec_base)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_library(mgs_codec_base INTERFACE)
target_include_directories(mgs_codec_base INTERFACE include)
target_link_libraries(mgs_codec_base INTERFACE CONAN_PKG::mgs_meta)

include(CTest)

if (BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

install(TARGETS mgs_codec_base
  EXPORT mgs_codec_base
)

install(DIRECTORY include DESTINATION .)
