cmake_minimum_required(VERSION 3.3)

add_executable(test_ranges
  test_input_transformer.cpp
  test_readable_transformed_input_range.cpp
  test_sized_input_transformer.cpp
  test_sized_transformed_input_range.cpp
  test_transformed_input_range.cpp
  test_transformed_input_range_iterator.cpp
  $<TARGET_OBJECTS:catch_main>)

target_link_libraries(test_ranges ranges mgs_test_helpers CONAN_PKG::mgs_meta)

add_test(NAME test_ranges COMMAND test_ranges)
