#include <algorithm>
#include <chrono>
#include <deque>
#include <forward_list>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <type_traits>
#include <vector>

#include <catch.hpp>

#include <mgs/detail/meta/concepts/derived_from.hpp>
#include <mgs/detail/meta/concepts/encoder.hpp>
#include <mgs/detail/meta/concepts/iterable.hpp>
#include <mgs/encoders/base64_stream.hpp>

using namespace std::string_literals;
using namespace mgs;

extern std::vector<std::string> testFilePaths;

namespace
{
template <typename Iterator, typename Sentinel = Iterator>
using encoder = encoders::base64_stream_encoder<Iterator, Sentinel>;

static_assert(detail::is_encoder<encoder<char*>>::value, "");
static_assert(detail::is_encoder<encoder<std::list<char>::iterator>>::value,
              "");
static_assert(
    detail::is_encoder<encoder<std::forward_list<char>::iterator>>::value, "");
static_assert(
    detail::is_encoder<encoder<std::istreambuf_iterator<char>>>::value, "");

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

template <typename Iterable>
void expect_base64(Iterable it, std::string expected_b64)
{
  using Iterator = detail2::result_of_begin_t<Iterable>;
  using Sentinel = detail2::result_of_end_t<Iterable>;
  using std::begin;
  using std::end;

  encoders::base64_stream_encoder<Iterator, Sentinel> enc(begin(it), end(it));

  std::string s(enc.begin(), enc.end());
  CHECK(s == expected_b64);
}

template <typename Container>
void base64_checks()
{
  Container two_byte_padding{'a', 'b', 'c', 'd'};
  Container one_byte_padding{'a', 'b', 'c', 'd', 'e'};
  Container no_padding{'a', 'b', 'c', 'd', 'e', 'f'};

  expect_base64(two_byte_padding, "YWJjZA==");
  expect_base64(one_byte_padding, "YWJjZGU=");
  expect_base64(no_padding, "YWJjZGVm");
}
}

TEST_CASE("b64 stream", "[encoding][base64]")
{
  SECTION("RandomAccessIterator")
  {
    SECTION("std::string")
    {
      base64_checks<std::string>();
    }

    SECTION("std::vector")
    {
      base64_checks<std::vector<char>>();
    }

    SECTION("std::deque")
    {
      base64_checks<std::deque<char>>();
    }

    SECTION("std::array")
    {
      std::array<char, 4> two_byte_padding{'a', 'b', 'c', 'd'};
      std::array<char, 5> one_byte_padding{'a', 'b', 'c', 'd', 'e'};
      std::array<char, 6> no_padding{'a', 'b', 'c', 'd', 'e', 'f'};

      expect_base64(two_byte_padding, "YWJjZA==");
      expect_base64(one_byte_padding, "YWJjZGU=");
      expect_base64(no_padding, "YWJjZGVm");
    }
  }

  SECTION("BidirectionalIterator")
  {
    SECTION("std::list")
    {
      base64_checks<std::list<char>>();
    }
  }

  SECTION("ForwardIterator")
  {
    SECTION("std::forward_list")
    {
      base64_checks<std::forward_list<char>>();
    }
  }

  SECTION("InputIterator")
  {
    SECTION("std::ifstream")
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
  }

  SECTION("Inception")
  {
    auto const text = "abcde"s;
    auto const b64_text = "YWJjZGU="s;
    auto const final_b64_text = "WVdKalpHVT0="s;

    encoder<std::string::const_iterator> first_enc(text.begin(), text.end());
    encoder<decltype(first_enc.begin())> second_enc(first_enc.begin(), first_enc.end());

    std::string s(second_enc.begin(), second_enc.end());
    CHECK(s == final_b64_text);
  }
}
