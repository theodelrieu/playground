cmake_minimum_required(VERSION 3.1.2)

add_executable(test_codec_base test_codec_base.cpp $<TARGET_OBJECTS:catch_main>)
target_link_libraries(test_codec_base mgs_codec_base CONAN_PKG::mgs_test_helpers)
