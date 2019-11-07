cmake_minimum_required(VERSION 3.3)

add_executable(test_binary_to_base test_binary_to_base.cpp $<TARGET_OBJECTS:catch_main>)
target_link_libraries(test_binary_to_base binary_to_base CONAN_PKG::mgs_meta mgs_test_helpers)

add_test(NAME test_binary_to_base COMMAND test_binary_to_base)
