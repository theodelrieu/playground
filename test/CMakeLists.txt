cmake_minimum_required(VERSION 3.1.2)

add_executable(test_b64 main.cpp test_b64.cpp)

target_link_libraries(test_b64 b64 CONAN_PKG::catch2)

include(ExternalData)

set(ExternalData_URL_TEMPLATES 
  "https://github.com/theodelrieu/cmake_external_data/raw/master/base64/%(algo)/%(hash)"
)

ExternalData_Add_Test(b64_data NAME test_b64 COMMAND test_b64 --test-file DATA{data/random_data} --test-file DATA{data/random_data.b64})

ExternalData_Add_Target(b64_data)
