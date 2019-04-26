set(CMAKE_CXX_COMPILER_WORKS 1)
cmake_minimum_required(VERSION 3.3)
project(base64url VERSION 0.1.0 LANGUAGES CXX)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS NO_OUTPUT_DIRS)

add_library(base64url INTERFACE)
target_include_directories(base64url INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(base64url INTERFACE CONAN_PKG::mgs_binary_to_text CONAN_PKG::mgs_codecs CONAN_PKG::mgs_ranges)

include(CTest)

if (BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

include(MgsExportAndInstallTarget)

mgs_export_and_install_target(base64url)
