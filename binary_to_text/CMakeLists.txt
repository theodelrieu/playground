set(CMAKE_CXX_COMPILER_WORKS 1)
cmake_minimum_required(VERSION 3.3)
project(binary_to_text VERSION 0.1 LANGUAGES CXX)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_library(binary_to_text INTERFACE)
target_include_directories(binary_to_text INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(binary_to_text INTERFACE CONAN_PKG::mgs_meta CONAN_PKG::mgs_exceptions CONAN_PKG::mgs_codecs_base CONAN_PKG::mgs_concepts)

include(CTest)

if (BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

include(MgsExportAndInstallTarget)

mgs_export_and_install_target(binary_to_text)
