cmake_minimum_required(VERSION 3.3)
project(exceptions VERSION 0.1 LANGUAGES CXX)

add_library(exceptions INTERFACE)

include(MgsExportAndInstallTarget)

mgs_export_and_install_target(exceptions)
