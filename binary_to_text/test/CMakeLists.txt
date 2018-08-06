cmake_minimum_required(VERSION 3.1.2)

add_executable(test_binary_to_text test_binary_to_text.cpp $<TARGET_OBJECTS:catch_main>)
target_link_libraries(test_binary_to_text mgs_binary_to_text_test_helpers CONAN_PKG::mgs_meta)

add_test(NAME test_binary_to_text COMMAND test_binary_to_text)
