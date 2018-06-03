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
#include <mgs/encoders/base64_lazy.hpp>
#include <mgs/decoders/base64_lazy.hpp>

using namespace std::string_literals;
using namespace mgs;

extern std::vector<std::string> testFilePaths;

namespace
{
template <typename Iterator, typename Sentinel = Iterator>
using encoder = encoders::base64_lazy_encoder<Iterator, Sentinel>;

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

  encoders::base64_lazy_encoder<Iterator, Sentinel> enc(begin(it), end(it));

  std::string s(enc.begin(), enc.end());
  CHECK(s == expected_b64);
}

template <typename Container, typename Iterable>
void base64_checks(Iterable const& source, std::string const& expected)
{
  using std::begin;
  using std::end;

  Container cont(begin(source), end(source));
  expect_base64(cont, expected);
}
}

TEST_CASE("b64 lazy encoding", "[encoding][base64]")
{
  std::array<char, 4> two_byte_padding = {'a', 'b', 'c', 'd'};
  std::array<char, 5> one_byte_padding = {'a', 'b', 'c', 'd', 'e'};
  std::array<char, 6> no_padding = {'a', 'b', 'c', 'd', 'e', 'f'};

  auto const encoded_two_byte_padding = "YWJjZA=="s;
  auto const encoded_one_byte_padding = "YWJjZGU="s;
  auto const encoded_no_padding = "YWJjZGVm"s;

  SECTION("RandomAccessIterator")
  {
    SECTION("std::string")
    {
      base64_checks<std::string>(two_byte_padding, encoded_two_byte_padding);
      base64_checks<std::string>(one_byte_padding, encoded_one_byte_padding);
      base64_checks<std::string>(no_padding, encoded_no_padding);
    }

    SECTION("std::vector")
    {
      base64_checks<std::vector<char>>(two_byte_padding,
                                       encoded_two_byte_padding);
      base64_checks<std::vector<char>>(one_byte_padding,
                                       encoded_one_byte_padding);
      base64_checks<std::vector<char>>(no_padding, encoded_no_padding);
    }

    SECTION("std::deque")
    {
      base64_checks<std::deque<char>>(two_byte_padding,
                                      encoded_two_byte_padding);
      base64_checks<std::deque<char>>(one_byte_padding,
                                      encoded_one_byte_padding);
      base64_checks<std::deque<char>>(no_padding, encoded_no_padding);
    }

    SECTION("std::array")
    {
      expect_base64(two_byte_padding, encoded_two_byte_padding);
      expect_base64(one_byte_padding, encoded_one_byte_padding);
      expect_base64(no_padding, encoded_no_padding);
    }
  }

  SECTION("BidirectionalIterator")
  {
    SECTION("std::list")
    {
      base64_checks<std::list<char>>(two_byte_padding,
                                     encoded_two_byte_padding);
      base64_checks<std::list<char>>(one_byte_padding,
                                     encoded_one_byte_padding);
      base64_checks<std::list<char>>(no_padding, encoded_no_padding);
    }
  }

  SECTION("ForwardIterator")
  {
    SECTION("std::forward_list")
    {
      base64_checks<std::forward_list<char>>(two_byte_padding,
                                             encoded_two_byte_padding);
      base64_checks<std::forward_list<char>>(one_byte_padding,
                                             encoded_one_byte_padding);
      base64_checks<std::forward_list<char>>(no_padding, encoded_no_padding);
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
      encoders::base64_lazy_encoder<decltype(input.begin())> enc(
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

TEST_CASE("b64 lazy decoding", "[decoding][base64]")
{
  auto const encoded_no_padding = "YWJjZGVm"s;

  decoders::base64_lazy_decoder<std::string::const_iterator> dec(
      encoded_no_padding.begin(), encoded_no_padding.end());

  std::string s(dec.begin(), dec.end());

  CHECK(s == "abcdef");
}
