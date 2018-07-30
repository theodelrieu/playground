cmake_minimum_required(VERSION 3.1.2)

add_executable(test_base_n test_base_n.cpp $<TARGET_OBJECTS:catch_main>)
target_link_libraries(test_base_n mgs_base_n CONAN_PKG::mgs_test_helpers CONAN_PKG::mgs_meta)

add_test(test_base_n NAME test_base_n COMMAND test_base_n)
