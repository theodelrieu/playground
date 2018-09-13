cmake_minimum_required(VERSION 3.1.2)
project(base16 VERSION 0.1 LANGUAGES CXX)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_library(base16 INTERFACE)
target_link_libraries(base16 INTERFACE CONAN_PKG::mgs_binary_to_text CONAN_PKG::mgs_codecs_base CONAN_PKG::mgs_adapters)

include(CTest)

if (BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

include(MgsExportAndInstallTarget)

mgs_export_and_install_target(base16)
