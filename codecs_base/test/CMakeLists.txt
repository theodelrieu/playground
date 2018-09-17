cmake_minimum_required(VERSION 3.3)

add_library(codecs_base_test_helpers INTERFACE)
target_include_directories(codecs_base_test_helpers INTERFACE include)
target_link_libraries(codecs_base_test_helpers INTERFACE codecs_base)

install(TARGETS codecs_base_test_helpers
  EXPORT codecs_base_test_helpers
  EXCLUDE_FROM_ALL
  PUBLIC_HEADER
    DESTINATION include
)

add_executable(test_codecs_base test_codecs_base.cpp $<TARGET_OBJECTS:catch_main>)
target_link_libraries(test_codecs_base codecs_base codecs_base_test_helpers CONAN_PKG::mgs_adapters CONAN_PKG::mgs_test_helpers)

add_test(NAME test_codecs_base COMMAND test_codecs_base)
