cmake_minimum_required(VERSION 3.1.2)

add_executable(test_base32 test_base32.cpp $<TARGET_OBJECTS:catch_main>)
target_link_libraries(test_base32 mgs_base32 mgs_binary_to_text_test_helpers CONAN_PKG::mgs_meta)

include(ExternalData)

set(ExternalData_URL_TEMPLATES
  "https://github.com/theodelrieu/cmake_external_data/raw/master/binary/%(algo)/%(hash)"
  "https://github.com/theodelrieu/cmake_external_data/raw/master/base32/%(algo)/%(hash)"
)

ExternalData_Add_Test(base32_data NAME test_base32 COMMAND test_base32 --test-file DATA{data/random_data} --test-file DATA{data/random_data.b32})
ExternalData_Add_Target(base32_data)

add_dependencies(test_base32 base32_data)
