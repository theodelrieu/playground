#include <algorithm>
#include <fstream>
#include <iterator>
#include <type_traits>
#include <vector>

#include <catch.hpp>

#include <b64/encoders/base64_stream.hpp>

using namespace std::string_literals;

extern std::vector<std::string> testFilePaths;

namespace
{
// streams are not Iterable until C++20.
struct stream_iterable_adapter
{
  stream_iterable_adapter(std::istream& is): _is(is){}

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

TEST_CASE("b64 encode", "[b64]")
{
  SECTION("Two bytes of padding")
  {
    auto const text = "abcd"s;
    b64::encoders::base64_stream_encoder<std::string::const_iterator> pr(
        text.begin(), text.end());

    std::string s(pr.begin(), pr.end());
    CHECK(s == "YWJjZA==");
  }

  SECTION("One byte of padding")
  {
    auto const text = "abcde"s;
    b64::encoders::base64_stream_encoder<std::string::const_iterator> pr(
        text.begin(), text.end());

    std::string s(pr.begin(), pr.end());
    CHECK(s == "YWJjZGU=");
  }

  SECTION("No padding")
  {
    auto const text = "abcdef"s;
    b64::encoders::base64_stream_encoder<std::string::const_iterator> pr(
        text.begin(), text.end());

    std::string s(pr.begin(), pr.end());
    CHECK(s == "YWJjZGVm");
  }

  SECTION("huge file")
  {
    REQUIRE(testFilePaths.size() == 2);
    std::ifstream random_data(testFilePaths[0]);
    std::ifstream b64_random_data(testFilePaths[1]);

    stream_iterable_adapter input{random_data};
    b64::encoders::base64_stream_encoder<decltype(input.begin())> pr(
        input.begin(), input.end());
    std::istreambuf_iterator<char> expectedB64It(b64_random_data);
    std::istreambuf_iterator<char> expectedEnd;
    CHECK(std::equal(expectedB64It, expectedEnd, pr.begin(), pr.end()));
  }
}
