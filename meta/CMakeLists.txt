cmake_minimum_required(VERSION 3.1.2)
project(meta VERSION 0.1 LANGUAGES CXX)

include(MgsExportAndInstallTarget)

add_library(meta INTERFACE)

mgs_export_and_install_target(meta)

if (BUILD_TESTING)
  add_subdirectory(test)
endif()
