cmake_minimum_required(VERSION 3.1.2)
project(base64 VERSION 0.1 LANGUAGES CXX)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_library(base64 INTERFACE)
target_link_libraries(base64 INTERFACE CONAN_PKG::mgs_binary_to_text CONAN_PKG::mgs_adapters CONAN_PKG::mgs_codecs_base)

include(CTest)

if (BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

include(MgsExportAndInstallTarget)

mgs_export_and_install_target(base64)
