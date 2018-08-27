cmake_minimum_required(VERSION 3.1.2)
project(iterators VERSION 0.1 LANGUAGES CXX)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

include(MgsExportAndInstallTarget)

add_library(iterators INTERFACE)
target_link_libraries(iterators INTERFACE CONAN_PKG::mgs_meta CONAN_PKG::mgs_adapters_base)

include(CTest)

if (BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

mgs_export_and_install_target(iterators)
