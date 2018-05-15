#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <type_traits>
#include <vector>

#include <catch.hpp>

#include <b64/encoders/base64_stream.hpp>

using namespace std::string_literals;
using namespace b64;

extern std::vector<std::string> testFilePaths;

namespace
{
// streams are not Iterable until C++20.
struct stream_iterable_adapter
{
  stream_iterable_adapter(std::istream& is) : _is(is)
  {
  }

  auto begin() const
  {
    return std::istreambuf_iterator<char>{_is};
  }

  auto end() const
  {
    return std::istreambuf_iterator<char>{};
  }

  std::istream& _is;
};
}

TEST_CASE("b64 stream", "[encoding][base64]")
{
  SECTION("Two bytes of padding")
  {
    auto const text = "abcd"s;
    encoders::base64_stream_encoder<std::string::const_iterator> enc(
        text.begin(), text.end());

    std::string s(enc.begin(), enc.end());
    CHECK(s == "YWJjZA==");
  }

  SECTION("One byte of padding")
  {
    auto const text = "abcde"s;
    encoders::base64_stream_encoder<std::string::const_iterator> enc(
        text.begin(), text.end());

    std::string s(enc.begin(), enc.end());
    CHECK(s == "YWJjZGU=");
  }

  SECTION("No padding")
  {
    auto const text = "abcdef"s;
    encoders::base64_stream_encoder<std::string::const_iterator> enc(
        text.begin(), text.end());

    std::string s(enc.begin(), enc.end());
    CHECK(s == "YWJjZGVm");
  }

  SECTION("Huge file")
  {
    REQUIRE(testFilePaths.size() == 2);
    std::ifstream random_data(testFilePaths[0]);
    std::ifstream b64_random_data(testFilePaths[1]);

    stream_iterable_adapter input{random_data};
    encoders::base64_stream_encoder<decltype(input.begin())> enc(
        input.begin(), input.end());
    std::istreambuf_iterator<char> expectedB64It(b64_random_data);
    std::istreambuf_iterator<char> expectedEnd;
    CHECK(std::equal(expectedB64It, expectedEnd, enc.begin(), enc.end()));
  }

  SECTION("Iterators")
  {
    auto const text = "abcdefghijklmnopqrstuvwxyz"s;
    auto const b64_text = "YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXo="s;

    encoders::base64_stream_encoder<std::string::const_iterator> enc(
        text.begin(), text.end());

    auto begin = enc.begin();
    auto const end = enc.end();

    static_assert(
        std::is_same<
            std::random_access_iterator_tag,
            std::iterator_traits<decltype(begin)>::iterator_category>::value,
        "");

    SECTION("Seek forward")
    {
      auto copy_it = begin;

      CHECK(*(begin + 4) == 'Z'); 
      CHECK(*(copy_it++) == 'Y'); 
      CHECK(*(begin + 20) == 'c');
      ++copy_it;
      CHECK(*copy_it == 'J');
      CHECK(*(begin + 35) == '=');
      CHECK(begin + 36 == end);
    }
  }
}
