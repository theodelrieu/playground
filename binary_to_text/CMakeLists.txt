cmake_minimum_required(VERSION 3.1.2)
project(mgs_binary_to_text)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_library(mgs_binary_to_text INTERFACE)
target_include_directories(mgs_binary_to_text INTERFACE include)
target_link_libraries(mgs_binary_to_text INTERFACE CONAN_PKG::boost_container CONAN_PKG::mgs_meta CONAN_PKG::mgs_exceptions)

include(CTest)

if (BUILD_TESTING)
  add_library(mgs_binary_to_text_test_helpers INTERFACE)
  target_include_directories(mgs_binary_to_text_test_helpers INTERFACE test/include)
  target_link_libraries(mgs_binary_to_text_test_helpers INTERFACE mgs_binary_to_text CONAN_PKG::mgs_adapters CONAN_PKG::catch2)

  install(TARGETS mgs_binary_to_text_test_helpers
    EXPORT mgs_binary_to_text_test_helpers
    EXCLUDE_FROM_ALL
    PUBLIC_HEADER
      DESTINATION include
  )

  enable_testing()
  add_subdirectory(test)
endif()

install(TARGETS mgs_binary_to_text
  EXPORT mgs_binary_to_text
)

install(DIRECTORY include DESTINATION .)
