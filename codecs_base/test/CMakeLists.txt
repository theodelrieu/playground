cmake_minimum_required(VERSION 3.1.2)

add_library(mgs_codecs_base_test_helpers INTERFACE)
target_include_directories(mgs_codecs_base_test_helpers INTERFACE include)
target_link_libraries(mgs_codecs_base_test_helpers INTERFACE mgs_codecs_base)

install(TARGETS mgs_codecs_base_test_helpers
  EXPORT mgs_codecs_base_test_helpers
  EXCLUDE_FROM_ALL
  PUBLIC_HEADER
    DESTINATION include
)

add_executable(test_codecs_base test_codecs_base.cpp $<TARGET_OBJECTS:catch_main>)
target_link_libraries(test_codecs_base mgs_codecs_base mgs_codecs_base_test_helpers CONAN_PKG::mgs_adapters CONAN_PKG::mgs_test_helpers)

add_test(NAME test_codecs_base COMMAND test_codecs_base)
