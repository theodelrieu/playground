cmake_minimum_required(VERSION 3.1.2)

add_library(catch_main OBJECT main.cpp)
get_target_property(CATCH2_INC_DIR CONAN_PKG::catch2 INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(catch_main PRIVATE ${CATCH2_INC_DIR})

function(create_tests)
  foreach(name IN LISTS ARGV)
    add_executable(test_${name} test_${name}.cpp $<TARGET_OBJECTS:catch_main>)
    target_link_libraries(test_${name} mgs CONAN_PKG::catch2)
  endforeach()
endfunction(create_tests)

create_tests(base64 base32 base16 adaptive_iterator)

include(ExternalData)

set(ExternalData_URL_TEMPLATES 
  "https://github.com/theodelrieu/cmake_external_data/raw/master/binary/%(algo)/%(hash)"
  "https://github.com/theodelrieu/cmake_external_data/raw/master/base64/%(algo)/%(hash)"
  "https://github.com/theodelrieu/cmake_external_data/raw/master/base32/%(algo)/%(hash)"
  "https://github.com/theodelrieu/cmake_external_data/raw/master/base16/%(algo)/%(hash)"
)

ExternalData_Add_Test(base64_data NAME test_base64 COMMAND test_base64 --test-file DATA{data/random_data} --test-file DATA{data/random_data.b64})
ExternalData_Add_Test(base32_data NAME test_base32 COMMAND test_base32 --test-file DATA{data/random_data} --test-file DATA{data/random_data.b32})
ExternalData_Add_Test(base16_data NAME test_base16 COMMAND test_base16 --test-file DATA{data/random_data} --test-file DATA{data/random_data.b16})

ExternalData_Add_Target(base64_data)
ExternalData_Add_Target(base32_data)
ExternalData_Add_Target(base16_data)

add_test(NAME test_adaptive_iterator COMMAND test_adaptive_iterator)
