cmake_minimum_required(VERSION 3.3)

add_library(binary_to_text_test_helpers INTERFACE)
target_include_directories(binary_to_text_test_helpers INTERFACE include)
target_link_libraries(binary_to_text_test_helpers INTERFACE binary_to_text codecs_base_test_helpers CONAN_PKG::mgs_adapters CONAN_PKG::mgs_test_helpers)

install(TARGETS binary_to_text_test_helpers
  EXPORT binary_to_text_test_helpers
  EXCLUDE_FROM_ALL
  PUBLIC_HEADER
    DESTINATION include
)

add_executable(test_binary_to_text test_binary_to_text.cpp $<TARGET_OBJECTS:catch_main>)
target_link_libraries(test_binary_to_text binary_to_text_test_helpers CONAN_PKG::mgs_meta)

add_test(NAME test_binary_to_text COMMAND test_binary_to_text)
