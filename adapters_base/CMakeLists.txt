cmake_minimum_required(VERSION 3.1.2)
project(adapters_base VERSION 0.1 LANGUAGES CXX)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

include(MgsExportAndInstallTarget)

add_library(adapters_base INTERFACE)
target_link_libraries(adapters_base INTERFACE CONAN_PKG::mgs_meta)

mgs_export_and_install_target(adapters_base)
