cmake_minimum_required(VERSION 3.3)

add_executable(test_codecs_base
  test_codecs_base.cpp
  test_codec_output.cpp
  test_codec_traits.cpp
  test_codec.cpp
  $<TARGET_OBJECTS:catch_main>
)

target_link_libraries(test_codecs_base codecs_base CONAN_PKG::mgs_ranges CONAN_PKG::mgs_test_helpers)

add_test(NAME test_codecs_base COMMAND test_codecs_base)
