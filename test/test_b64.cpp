#include <algorithm>
#include <fstream>
#include <iostream>

#include <catch.hpp>

#include <b64/encode_iterator.hpp>

using namespace std::string_literals;

TEST_CASE("b64 encode", "[b64]")
{
  SECTION("Two bytes of padding")
  {
    auto const text = "abcd"s;

    b64::base64_encoding_iterator<std::string::const_iterator> it(
        std::begin(text), std::end(text));

    decltype(it) end;

    std::string s(it, end);
    CHECK(s == "YWJjZA==");
  }

  SECTION("One byte of padding")
  {
    auto const text = "abcde"s;

    b64::base64_encoding_iterator<std::string::const_iterator> it(
        std::begin(text), std::end(text));

    decltype(it) end;

    std::string s(it, end);
    CHECK(s == "YWJjZGU=");
  }

  SECTION("No padding")
  {
    auto const text = "abcdef"s;

    b64::base64_encoding_iterator<std::string::const_iterator> it(
        std::begin(text), std::end(text));

    decltype(it) end;

    std::string s(it, end);
    CHECK(s == "YWJjZGVm");
  }

  SECTION("huge file")
  {
    std::ifstream random_data("/Users/theo/Projects/b64iterator/test/data/random_data");
    std::ifstream b64_random_data("/Users/theo/Projects/b64iterator/test/data/random_data.b64");

    b64::base64_encoding_iterator<std::istreambuf_iterator<char>> it{
        std::istreambuf_iterator<char>(random_data),
        std::istreambuf_iterator<char>()};
    b64::base64_encoding_iterator<std::istreambuf_iterator<char>> end;

    std::istreambuf_iterator<char> expectedB64It(b64_random_data);
    std::istreambuf_iterator<char> expectedEnd;
    // std::ofstream lol("lol");
    // std::string s(it, end);
    // lol << s;
    CHECK(std::equal(expectedB64It, expectedEnd, it, end));
  }
}
