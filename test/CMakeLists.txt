cmake_minimum_required(VERSION 3.1.2)

add_executable(test_b64 main.cpp test_b64.cpp)

target_link_libraries(test_b64 b64 CONAN_PKG::catch2)

add_test(NAME test_b64 COMMAND test_b64)
