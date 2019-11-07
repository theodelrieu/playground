cmake_minimum_required(VERSION 3.3)

add_library(catch_main OBJECT catch_main.cpp)
get_target_property(CATCH2_INC_DIR CONAN_PKG::Catch2 INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(catch_main PRIVATE ${CATCH2_INC_DIR})

install(TARGETS catch_main
  EXPORT catch_main
  EXCLUDE_FROM_ALL
  OBJECTS
    DESTINATION bin
)

add_executable(test_codecs
  test_codecs.cpp
  test_codec_output.cpp
  test_codec_traits.cpp
  test_byte_type.cpp
  test_input_source.cpp
  test_sized_input_source.cpp
  test_basic_input_range_iterator.cpp
  test_input_source_view.cpp
  test_codec.cpp
  $<TARGET_OBJECTS:catch_main>
)

add_library(mgs_codec_helpers INTERFACE)
target_include_directories(mgs_codec_helpers INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(test_codecs codecs CONAN_PKG::mgs_ranges CONAN_PKG::Catch2 mgs_codec_helpers)

add_test(NAME test_codecs COMMAND test_codecs)
