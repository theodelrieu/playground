#include <algorithm>
#include <chrono>
#include <deque>
#include <forward_list>
#include <fstream>
#include <iterator>
#include <list>
#include <sstream>
#include <type_traits>
#include <vector>

#include <catch.hpp>

#include <mgs/detail/base64/lazy_encoder.hpp>
#include <mgs/detail/base64/lazy_decoder.hpp>
#include <mgs/detail/meta/concepts/derived_from.hpp>
#include <mgs/detail/meta/concepts/iterable.hpp>
#include <mgs/detail/meta/concepts/iterable_input_transformer.hpp>
#include <mgs/exceptions/parse_error.hpp>

using namespace std::string_literals;
using namespace mgs;

extern std::vector<std::string> testFilePaths;

namespace
{
template <typename Iterator, typename Sentinel = Iterator>
using b64_encoder = detail::base64_lazy_encoder<Iterator, Sentinel>;

template <typename Iterator, typename Sentinel = Iterator>
using b64_decoder = detail::base64_lazy_decoder<Iterator, Sentinel>;

static_assert(
    detail::is_iterable_input_transformer<b64_encoder<char*>>::value, "");
static_assert(detail::is_iterable_input_transformer<
                  b64_encoder<std::list<char>::iterator>>::value,
              "");
static_assert(detail::is_iterable_input_transformer<
                  b64_encoder<std::forward_list<char>::iterator>>::value,
              "");
static_assert(detail::is_iterable_input_transformer<
                  b64_encoder<std::istreambuf_iterator<char>>>::value,
              "");

struct encode_tag
{
};

struct decode_tag
{
};

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

struct sentinel
{
};

bool operator==(std::istreambuf_iterator<char> lhs, sentinel) noexcept
{
  return lhs.equal({});
}

bool operator==(sentinel, std::istreambuf_iterator<char> rhs) noexcept
{
  return rhs.equal({});
}

bool operator!=(std::istreambuf_iterator<char> lhs, sentinel s) noexcept
{
  return !(lhs == s);
}

bool operator!=(sentinel s, std::istreambuf_iterator<char> rhs) noexcept
{
  return !(s == rhs);
}

static_assert(
    detail::is_sentinel<sentinel, std::istreambuf_iterator<char>>::value, "");

static_assert(
    !detail::is_sentinel<sentinel, std::vector<char>::iterator>::value, "");

template <typename EncodingTraits, typename Container, typename Iterable>
void base_n_checks_impl(Container const& source,
                        Iterable const& expected_output)
{
  using Iterator = detail2::result_of_begin_t<Container>;
  using Sentinel = detail2::result_of_end_t<Container>;

  using std::begin;
  using std::end;

  detail::base_n_transformer<EncodingTraits, Iterator, Sentinel> transformer(
      begin(source), end(source));

  std::vector<std::uint8_t> const output(transformer.begin(),
                                         transformer.end());
  std::vector<std::uint8_t> const expected(expected_output.begin(),
                                           expected_output.end());

  CHECK(output == expected);
}

template <typename EncodingTraits,
          typename Container,
          typename Iterable,
          typename Iterable2>
void base_n_checks(Iterable const& source, Iterable2 const& expected)
{
  using std::begin;
  using std::end;

  Container cont(begin(source), end(source));

  base_n_checks_impl<EncodingTraits>(cont, expected);
}

template <typename EncodingTraits,
          typename Container,
          typename Input,
          typename Output>
void base_n_checks(std::vector<Input> const& inputs,
                   std::vector<Output> const& outputs)
{
  REQUIRE(inputs.size() == outputs.size());

  for (auto i = 0; i < inputs.size(); ++i)
    base_n_checks<EncodingTraits, Container>(inputs[i], outputs[i]);
}

template <typename EncodingTraits, typename Input, typename Output>
void common_checks(std::vector<Input> const& inputs,
                   std::vector<Output> const& outputs)
{
  SECTION("RandomAccessIterator")
  {
    SECTION("std::string")
    {
      base_n_checks<EncodingTraits, std::string>(inputs, outputs);
    }

    SECTION("std::vector")
    {
      base_n_checks<EncodingTraits, std::vector<char>>(inputs, outputs);
    }

    SECTION("std::deque")
    {
      base_n_checks<EncodingTraits, std::deque<char>>(inputs, outputs);
    }
  }

  SECTION("BidirectionalIterator")
  {
    SECTION("std::list")
    {
      base_n_checks<EncodingTraits, std::list<char>>(inputs, outputs);
    }
  }

  SECTION("ForwardIterator")
  {
    SECTION("std::forward_list")
    { 
      base_n_checks<EncodingTraits, std::forward_list<char>>(inputs, outputs);
    }
  }
}
}

TEST_CASE("b64 lazy", "[base64]")
{
  std::vector<std::string> decoded{"abcd"s, "abcde"s, "abcdef"s};
  std::vector<std::string> encoded{"YWJjZA=="s, "YWJjZGU="s, "YWJjZGVm"s};

  SECTION("encoding")
  {
    using Traits = detail::base64_encode_traits;

    SECTION("common_checks")
    {
      common_checks<Traits>(decoded, encoded);
    }

    SECTION("sentinel")
    {
      std::stringstream ss{"abcde"s};
      auto const b64_text = "YWJjZGU="s;

      b64_encoder<std::istreambuf_iterator<char>, sentinel> enc(
          std::istreambuf_iterator<char>(ss), sentinel{});

      std::string s(enc.begin(), enc.end());
      CHECK(s == b64_text);
    }

    SECTION("Inception")
    {
      auto const text = "abcde"s;
      auto const b64_text = "YWJjZGU="s;
      auto const final_b64_text = "WVdKalpHVT0="s;

      b64_encoder<std::string::const_iterator> first_enc(text.begin(), text.end());
      b64_encoder<decltype(first_enc.begin())> second_enc(first_enc.begin(),
                                                      first_enc.end());

      std::string s(second_enc.begin(), second_enc.end());
      CHECK(s == final_b64_text);
    }
  }

  SECTION("decoding")
  {
    using Traits = detail::base64_decode_traits;

    SECTION("common_checks")
    {
      common_checks<Traits>(encoded, decoded);
    }

    SECTION("sentinel")
    {
      std::stringstream ss{"YWJjZGU="s};
      auto const text = "abcde"s;

      b64_decoder<std::istreambuf_iterator<char>, sentinel> dec(
          std::istreambuf_iterator<char>(ss), sentinel{});

      std::string s(dec.begin(), dec.end());
      CHECK(s == text);
    }

    SECTION("Inception")
    {
      auto const text = "abcde"s;
      auto const b64_text = "YWJjZGU="s;
      auto const final_b64_text = "WVdKalpHVT0="s;

      b64_decoder<std::string::const_iterator> first_dec(final_b64_text.begin(),
                                                     final_b64_text.end());
      b64_decoder<decltype(first_dec.begin())> second_dec(first_dec.begin(),
                                                      first_dec.end());

      std::string s(second_dec.begin(), second_dec.end());
      CHECK(s == text);
    }
  }

  SECTION("back and forth")
  {
    SECTION("decode(encode())")
    {
      auto const text = "abcde"s;

      b64_encoder<std::string::const_iterator> enc(text.begin(), text.end());
      b64_decoder<decltype(enc.begin())> dec(enc.begin(), enc.end());

      std::string s(dec.begin(), dec.end());
      CHECK(s == text);
    }

    SECTION("encode(decode())")
    {
      auto const text = "YWJjZGU="s;

      b64_decoder<std::string::const_iterator> dec(text.begin(), text.end());
      b64_encoder<decltype(dec.begin())> enc(dec.begin(), dec.end());

      std::string s(enc.begin(), enc.end());
      CHECK(s == text);
    }
  }

  SECTION("stream")
  {
    REQUIRE(testFilePaths.size() == 2);
    std::ifstream random_data(testFilePaths[0]);
    std::ifstream b64_random_data(testFilePaths[1]);

    stream_iterable_adapter random_input{random_data};
    stream_iterable_adapter b64_random_input{b64_random_data};

    using EncoderTraits = detail::base64_encode_traits;
    using DecoderTraits = detail::base64_encode_traits;

    base_n_checks_impl<EncoderTraits>(random_input, b64_random_input);
    base_n_checks_impl<DecoderTraits>(b64_random_input, random_input);
  }
}
