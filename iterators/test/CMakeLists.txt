cmake_minimum_required(VERSION 3.1.2)

add_executable(test_adaptive_iterator test_adaptive_iterator.cpp $<TARGET_OBJECTS:catch_main>)
target_link_libraries(test_adaptive_iterator mgs_iterators CONAN_PKG::catch2)

add_test(NAME test_adaptive_iterator COMMAND test_adaptive_iterator)
