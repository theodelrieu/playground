cmake_minimum_required(VERSION 3.1.2)

add_library(catch_main OBJECT main.cpp)
get_target_property(CATCH2_INC_DIR CONAN_PKG::catch2 INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(catch_main PRIVATE ${CATCH2_INC_DIR})

add_executable(test_b64 test_b64.cpp $<TARGET_OBJECTS:catch_main>)
add_executable(test_adaptive_iterator test_adaptive_iterator.cpp $<TARGET_OBJECTS:catch_main>)
add_executable(test_wrap_integer test_wrap_integer.cpp $<TARGET_OBJECTS:catch_main>)

target_link_libraries(test_b64 b64 CONAN_PKG::catch2)
target_link_libraries(test_adaptive_iterator b64 CONAN_PKG::catch2)
target_link_libraries(test_wrap_integer b64 CONAN_PKG::catch2)
# TODO function/macro

include(ExternalData)

set(ExternalData_URL_TEMPLATES 
  "https://github.com/theodelrieu/cmake_external_data/raw/master/base64/%(algo)/%(hash)"
)

ExternalData_Add_Test(b64_data NAME test_b64 COMMAND test_b64 --test-file DATA{data/random_data} --test-file DATA{data/random_data.b64})

ExternalData_Add_Target(b64_data)

add_test(NAME test_adaptive_iterator COMMAND test_adaptive_iterator)
add_test(NAME test_wrap_integer COMMAND test_wrap_integer)
