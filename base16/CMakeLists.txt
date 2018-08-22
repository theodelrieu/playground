cmake_minimum_required(VERSION 3.1.2)
project(mgs_base16)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_library(mgs_base16 INTERFACE)
target_include_directories(mgs_base16 INTERFACE include)
target_link_libraries(mgs_base16 INTERFACE CONAN_PKG::mgs_binary_to_text CONAN_PKG::mgs_codecs_base CONAN_PKG::mgs_adapters)

include(CTest)

if (BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

install(TARGETS mgs_base16
  EXPORT mgs_base16
)

install(DIRECTORY include DESTINATION .)
