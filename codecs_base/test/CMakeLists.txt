cmake_minimum_required(VERSION 3.1.2)

add_executable(test_codecs_base test_codecs_base.cpp $<TARGET_OBJECTS:catch_main>)
target_link_libraries(test_codecs_base mgs_codecs_base CONAN_PKG::mgs_test_helpers)
