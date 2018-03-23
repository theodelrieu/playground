#include <algorithm>
#include <fstream>
#include <iostream>

#include <catch.hpp>

#include <b64/encode_iterator.hpp>
#include <b64/processors/stream_processor.hpp>

using namespace std::string_literals;

TEST_CASE("b64 encode", "[b64]")
{
  SECTION("Two bytes of padding")
  {
    auto const text = "abcd"s;
    b64::stream_processor<std::string::const_iterator> pr(text.begin(), text.end());
    b64::encoding_iterator<decltype(pr)> it(pr);
    decltype(it) end;

    std::string s(it, end);
    CHECK(s == "YWJjZA==");
  }

  SECTION("One byte of padding")
  {
    auto const text = "abcde"s;

    b64::stream_processor<std::string::const_iterator> pr(text.begin(), text.end());
    b64::encoding_iterator<decltype(pr)> it(pr);
    decltype(it) end;

    std::string s(it, end);
    CHECK(s == "YWJjZGU=");
  }

  SECTION("No padding")
  {
    auto const text = "abcdef"s;

    b64::stream_processor<std::string::const_iterator> pr(text.begin(), text.end());
    b64::encoding_iterator<decltype(pr)> it(pr);
    decltype(it) end;

    std::string s(it, end);
    CHECK(s == "YWJjZGVm");
  }

  SECTION("huge file")
  {
    // FIXME give to add_test the path, try to find in CMake how to download test files
    std::ifstream random_data("/Users/theo/Projects/b64iterator/test/data/random_data");
    std::ifstream b64_random_data("/Users/theo/Projects/b64iterator/test/data/random_data.b64");

    b64::stream_processor<std::istreambuf_iterator<char>> pr{
        std::istreambuf_iterator<char>(random_data),
        std::istreambuf_iterator<char>()};
    b64::encoding_iterator<decltype(pr)> it(pr);
    decltype(it) end;

    std::istreambuf_iterator<char> expectedB64It(b64_random_data);
    std::istreambuf_iterator<char> expectedEnd;
    CHECK(std::equal(expectedB64It, expectedEnd, it, end));
  }
}
