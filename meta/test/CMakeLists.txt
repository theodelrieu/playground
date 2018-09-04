cmake_minimum_required(VERSION 3.1.2)

function(add_meta_test name)
  add_executable(test_${name}
    test_${name}.cpp
    $<TARGET_OBJECTS:catch_main>
  )
  target_link_libraries(test_${name} mgs_meta CONAN_PKG::catch2)

  add_test(NAME test_${name} COMMAND test_${name})
endfunction()

add_meta_test(derived_from)
