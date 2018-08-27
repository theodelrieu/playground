cmake_minimum_required(VERSION 3.1.2)
project(adapters VERSION 0.1 LANGUAGES CXX)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_library(adapters INTERFACE)
target_link_libraries(adapters INTERFACE CONAN_PKG::mgs_meta CONAN_PKG::mgs_iterators CONAN_PKG::mgs_adapters_base)

include(MgsExportAndInstallTarget)
mgs_export_and_install_target(adapters)
