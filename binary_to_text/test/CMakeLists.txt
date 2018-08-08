cmake_minimum_required(VERSION 3.1.2)

add_library(mgs_binary_to_text_test_helpers INTERFACE)
target_include_directories(mgs_binary_to_text_test_helpers INTERFACE include)
target_link_libraries(mgs_binary_to_text_test_helpers INTERFACE mgs_binary_to_text CONAN_PKG::mgs_adapters CONAN_PKG::mgs_test_helpers)

install(TARGETS mgs_binary_to_text_test_helpers
  EXPORT mgs_binary_to_text_test_helpers
  EXCLUDE_FROM_ALL
  PUBLIC_HEADER
    DESTINATION include
)

add_executable(test_binary_to_text test_binary_to_text.cpp $<TARGET_OBJECTS:catch_main>)
target_link_libraries(test_binary_to_text mgs_binary_to_text_test_helpers CONAN_PKG::mgs_meta)

add_test(NAME test_binary_to_text COMMAND test_binary_to_text)
