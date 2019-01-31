cmake_minimum_required(VERSION 3.3)

function(create_concepts_tests)
  set(filenames "")
  foreach (name ${ARGN})
    list(APPEND filenames test_${name}.cpp)
  endforeach()

  add_executable(test_concepts
    ${filenames}
    $<TARGET_OBJECTS:catch_main>
  )
  target_link_libraries(test_concepts concepts CONAN_PKG::catch2 CONAN_PKG::mgs_meta CONAN_PKG::mgs_test_helpers)

  add_test(NAME test_concepts COMMAND test_concepts)
endfunction()

create_concepts_tests(
  byte
  input_transformer
  iterable
  sized_input_transformer
  transformed_input_range
)
