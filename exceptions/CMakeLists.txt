cmake_minimum_required(VERSION 3.3)
project(exceptions VERSION 0.1 LANGUAGES CXX)

add_library(exceptions INTERFACE)
target_include_directories(meta INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

include(MgsExportAndInstallTarget)

mgs_export_and_install_target(exceptions)
