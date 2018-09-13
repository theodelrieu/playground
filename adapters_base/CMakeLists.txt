cmake_minimum_required(VERSION 3.3)
project(adapters_base VERSION 0.1 LANGUAGES CXX)

find_package(mgs_meta REQUIRED)

include(MgsExportAndInstallTarget)

add_library(adapters_base INTERFACE)
target_include_directories(adapters_base INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(adapters_base INTERFACE mgs_meta)

mgs_export_and_install_target(adapters_base)
