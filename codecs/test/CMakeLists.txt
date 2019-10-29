cmake_minimum_required(VERSION 3.3)

add_executable(test_codecs
  test_codecs.cpp
  test_codec_output.cpp
  test_codec_traits.cpp
  test_byte_type.cpp
  test_input_source.cpp
  test_sized_input_source.cpp
  test_codec.cpp
  $<TARGET_OBJECTS:catch_main>
)

target_link_libraries(test_codecs codecs CONAN_PKG::mgs_ranges CONAN_PKG::mgs_test_helpers)

add_test(NAME test_codecs COMMAND test_codecs)
