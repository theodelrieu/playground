#include <algorithm>
#include <chrono>
#include <forward_list>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <type_traits>
#include <vector>

#include <catch.hpp>

#include <b64/detail/meta/concepts/bidirectional_encoder.hpp>
#include <b64/detail/meta/concepts/derived_from.hpp>
#include <b64/detail/meta/concepts/encoder.hpp>
#include <b64/detail/meta/concepts/random_access_encoder.hpp>
#include <b64/detail/meta/concepts/random_access_iterator.hpp>
#include <b64/encoders/base64_stream.hpp>

using namespace std::string_literals;
using namespace b64;

extern std::vector<std::string> testFilePaths;

namespace
{
template <typename Iterator, typename Sentinel = Iterator>
using encoder = encoders::base64_stream_encoder<Iterator, Sentinel>;

static_assert(detail::is_encoder<encoder<char*>>::value, "");
static_assert(detail::is_bidirectional_encoder<encoder<char*>>::value, "");
static_assert(detail::is_random_access_encoder<encoder<char*>>::value, "");

static_assert(!detail::is_random_access_encoder<
                  encoder<std::list<char>::iterator>>::value,
              "");
static_assert(
    detail::is_bidirectional_encoder<encoder<std::list<char>::iterator>>::value,
    "");
static_assert(detail::is_encoder<encoder<std::list<char>::iterator>>::value,
              "");

static_assert(!detail::is_random_access_encoder<
                  encoder<std::forward_list<char>::iterator>>::value,
              "");
static_assert(!detail::is_bidirectional_encoder<
                  encoder<std::forward_list<char>::iterator>>::value,
              "");
static_assert(
    detail::is_encoder<encoder<std::forward_list<char>::iterator>>::value, "");

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

template <typename Encoder>
void bidirectional_tests(Encoder const& enc, std::string const& b64_text)
{
  REQUIRE(b64_text.size() == 36);

  auto begin = enc.begin();
  auto const end = enc.end();

  static_assert(
      detail::is_derived_from<
          typename std::iterator_traits<decltype(begin)>::iterator_category,
          std::bidirectional_iterator_tag>::value,
      "");

  SECTION("Seek forward")
  {
    auto copy_it = begin;

    CHECK(*std::next(begin, 4) == b64_text[4]);
    CHECK(*(copy_it++) == b64_text[0]);
    CHECK(*std::next(begin, 20) == b64_text[20]);
    ++copy_it;
    CHECK(*copy_it == b64_text[2]);
    CHECK(*std::next(begin, 35) == b64_text.back());
    CHECK(std::next(begin, 36) == end);

    for (auto i = 0; i < b64_text.size(); ++i)
      CHECK(*std::next(begin, i) == b64_text[i]);
  }

  SECTION("Seek backward")
  {
    auto copy_it = std::next(begin, 35);

    CHECK(*copy_it == b64_text.back());
    CHECK(*(copy_it--) == b64_text.back());
    CHECK(*copy_it == b64_text[34]);
    CHECK(*std::prev(copy_it, 5) == b64_text[29]);
    CHECK(*std::prev(copy_it, 10) == b64_text[24]);
    std::advance(copy_it, -34);
    CHECK(copy_it == begin);
    CHECK(*copy_it == b64_text.front());

    auto copy_end = std::next(begin, 36);
    for (auto i = 0; i < b64_text.size(); ++i)
      CHECK(*std::prev(copy_end, b64_text.size() - i) == b64_text[i]);
  }
}
}

TEST_CASE("b64 stream", "[encoding][base64]")
{
  SECTION("Two bytes of padding")
  {
    auto const text = "abcd"s;
    encoders::base64_stream_encoder<std::string::const_iterator> enc(
        text.begin(), text.end());

    std::string s(enc.begin(), enc.end());
    static_assert(detail::is_random_access_iterator<decltype(s.begin())>::value,
                  "");

    static_assert(detail::is_random_access_iterator<decltype(enc.begin())>::value,
                  "");
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
    SECTION("One byte padding")
    {
      auto const text = "abcdefghijklmnopqrstuvwxyz"s;
      auto const b64_text = "YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXo="s;

      SECTION("Random access")
      {
        encoders::base64_stream_encoder<std::string::const_iterator> enc(
            text.begin(), text.end());

        bidirectional_tests(enc, b64_text);
      }

      SECTION("Bidirectional")
      {
        std::list<char> const l{text.begin(), text.end()};
        encoders::base64_stream_encoder<decltype(l.begin())> enc(l.begin(),
                                                                 l.end());

        bidirectional_tests(enc, b64_text);
      }
    }

    SECTION("Two byte padding")
    {
      auto const text = "abcdefghijklmnopqrstuvwxy"s;
      auto const b64_text = "YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eQ=="s;

      SECTION("Random access")
      {
        encoders::base64_stream_encoder<std::string::const_iterator> enc(
            text.begin(), text.end());

        bidirectional_tests(enc, b64_text);
      }

      SECTION("Bidirectional")
      {
        std::list<char> const l{text.begin(), text.end()};
        encoders::base64_stream_encoder<decltype(l.begin())> enc(l.begin(),
                                                                 l.end());

        bidirectional_tests(enc, b64_text);
      }
    }

    SECTION("No byte padding")
    {
      auto const text = "abcdefghijklmnopqrstuvwxyza"s;
      auto const b64_text = "YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXph"s;

      SECTION("Random access")
      {
        encoders::base64_stream_encoder<std::string::const_iterator> enc(
            text.begin(), text.end());

        bidirectional_tests(enc, b64_text);
      }

      SECTION("Bidirectional")
      {
        std::list<char> const l{text.begin(), text.end()};
        encoders::base64_stream_encoder<decltype(l.begin())> enc(l.begin(),
                                                                 l.end());

        bidirectional_tests(enc, b64_text);
      }
    }

    SECTION("reverse iterator")
    {
      auto const text = "abcdefghijklmnopqrstuvwxyza"s;
      auto b64_text = "YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXph"s;
      std::reverse(b64_text.begin(), b64_text.end());

      encoders::base64_stream_encoder<std::string::const_iterator> enc(
          text.begin(), text.end());

      auto rbegin = std::make_reverse_iterator(enc.end());
      auto const rend = std::make_reverse_iterator(enc.begin());

      std::string s(rbegin, rend);
      CHECK(s == b64_text);
    }
  }

  SECTION("Inception")
  {
    auto const text = "abcde"s;
    auto const b64_text = "YWJjZGU="s;
    auto const final_b64_text = "WVdKalpHVT0="s;

    encoder<std::string::const_iterator> first_enc(text.begin(), text.end());
    encoder<decltype(first_enc.begin())> second_enc(first_enc.begin(), first_enc.end());

    std::string s1(first_enc.begin(), first_enc.end());
    CHECK(s1 == b64_text);

    auto it = second_enc.begin();
    auto end = second_enc.end();
    auto dist = end - it;

    std::string s2(second_enc.begin(), second_enc.end());
    CHECK(s2 == final_b64_text);
  }

  // TODO test with sentinel once adaptive_iterator is refactored
}
