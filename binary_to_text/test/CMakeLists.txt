cmake_minimum_required(VERSION 3.3)

add_executable(test_binary_to_text test_binary_to_text.cpp $<TARGET_OBJECTS:catch_main>)
target_link_libraries(test_binary_to_text binary_to_text CONAN_PKG::mgs_meta CONAN_PKG::mgs_test_helpers)

add_test(NAME test_binary_to_text COMMAND test_binary_to_text)
