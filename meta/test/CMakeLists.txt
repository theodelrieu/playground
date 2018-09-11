cmake_minimum_required(VERSION 3.1.2)

function(create_meta_tests)
  set(filenames "")
  foreach (name ${ARGN})
    list(APPEND filenames test_${name}.cpp)
  endforeach()

  add_executable(test_meta
    ${filenames}
    $<TARGET_OBJECTS:catch_main>
  )
  target_link_libraries(test_meta mgs_meta CONAN_PKG::catch2)

  add_test(NAME test_meta COMMAND test_meta)
endfunction()

create_meta_tests(
  bidirectional_iterator
  complete_type
  derived_from
  equality_comparable
  forward_iterator
  incrementable
  input_iterator
  iterable
  iterator
  iterator_traits
  regular
  semiregular
  sentinel
  strict_totally_ordered
  swappable
)
