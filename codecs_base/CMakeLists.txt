cmake_minimum_required(VERSION 3.3)
project(codecs_base VERSION 0.1 LANGUAGES CXX)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_library(codecs_base INTERFACE)
target_link_libraries(codecs_base INTERFACE CONAN_PKG::mgs_adapters_base CONAN_PKG::mgs_exceptions CONAN_PKG::mgs_meta)

include(CTest)

if (BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

include(MgsExportAndInstallTarget)

mgs_export_and_install_target(codecs_base)
