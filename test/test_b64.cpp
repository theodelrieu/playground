#include <algorithm>
#include <chrono>
#include <deque>
#include <forward_list>
#include <fstream>
#include <iterator>
#include <list>
#include <type_traits>
#include <vector>

#include <catch.hpp>

#include <mgs/exceptions/parse_error.hpp>
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

template <typename Iterator, typename Sentinel = Iterator>
using decoder = decoders::base64_lazy_decoder<Iterator, Sentinel>;
// TODO is_encoder -> is_inpu_transformer + maybe a tag to have is_encoder/is_decoder?

static_assert(detail::is_encoder<encoder<char*>>::value, "");
static_assert(detail::is_encoder<encoder<std::list<char>::iterator>>::value,
              "");
static_assert(
    detail::is_encoder<encoder<std::forward_list<char>::iterator>>::value, "");
static_assert(
    detail::is_encoder<encoder<std::istreambuf_iterator<char>>>::value, "");

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

template <template <typename, typename, typename...> class InputTransformer,
          typename Container,
          typename Iterable>
void base64_checks_impl(Container const& source,
                        Iterable const& expected_output)
{
  using Iterator = detail2::result_of_begin_t<Container>;
  using Sentinel = detail2::result_of_end_t<Container>;

  using std::begin;
  using std::end;

  InputTransformer<Iterator, Sentinel> transformer(begin(source), end(source));
  std::vector<std::uint8_t> const output(transformer.begin(),
                                         transformer.end());
  std::vector<std::uint8_t> const expected(expected_output.begin(),
                                           expected_output.end());

  CHECK(output == expected);
}

template <typename Container, typename Iterable>
void base64_checks_impl(Container const& source,
                        Iterable const& expected_output,
                        encode_tag)
{
  base64_checks_impl<encoders::base64_lazy_encoder>(source, expected_output);
}

template <typename Container, typename Iterable>
void base64_checks_impl(Iterable const& expected_output,
                        Container const& source,
                        decode_tag)
{
  base64_checks_impl<decoders::base64_lazy_decoder>(source, expected_output);
}

template <typename Container,
          typename Iterable,
          typename Iterable2,
          typename Tag>
void base64_checks(Iterable const& source, Iterable2 const& expected, Tag tag)
{
  using std::begin;
  using std::end;

  Container cont(begin(source), end(source));

  base64_checks_impl(cont, expected, tag);
}

template <typename Tag>
void common_checks()
{
  Tag tag;

  std::array<char, 4> decoded_two_byte_padding = {'a', 'b', 'c', 'd'};
  std::array<char, 5> decoded_one_byte_padding = {'a', 'b', 'c', 'd', 'e'};
  std::array<char, 6> decoded_no_padding = {'a', 'b', 'c', 'd', 'e', 'f'};

  std::array<char, 8> const encoded_two_byte_padding{
      'Y', 'W', 'J', 'j', 'Z', 'A', '=', '='};
  std::array<char, 8> const encoded_one_byte_padding{
      'Y', 'W', 'J', 'j', 'Z', 'G', 'U', '='};
  std::array<char, 8> const encoded_no_padding{
      'Y', 'W', 'J', 'j', 'Z', 'G', 'V', 'm'};

  SECTION("RandomAccessIterator")
  {
    SECTION("std::string")
    {
      base64_checks<std::string>(
          decoded_two_byte_padding, encoded_two_byte_padding, tag);
      base64_checks<std::string>(
          decoded_one_byte_padding, encoded_one_byte_padding, tag);
      base64_checks<std::string>(decoded_no_padding, encoded_no_padding, tag);
    }

    SECTION("std::vector")
    {
      base64_checks<std::vector<char>>(
          decoded_two_byte_padding, encoded_two_byte_padding, tag);
      base64_checks<std::vector<char>>(
          decoded_one_byte_padding, encoded_one_byte_padding, tag);
      base64_checks<std::vector<char>>(
          decoded_no_padding, encoded_no_padding, tag);
    }

    SECTION("std::deque")
    {
      base64_checks<std::deque<char>>(
          decoded_two_byte_padding, encoded_two_byte_padding, tag);
      base64_checks<std::deque<char>>(
          decoded_one_byte_padding, encoded_one_byte_padding, tag);
      base64_checks<std::deque<char>>(
          decoded_no_padding, encoded_no_padding, tag);
    }

    SECTION("std::array")
    {
      base64_checks_impl(
          decoded_two_byte_padding, encoded_two_byte_padding, tag);
      base64_checks_impl(
          decoded_one_byte_padding, encoded_one_byte_padding, tag);
      base64_checks_impl(decoded_no_padding, encoded_no_padding, tag);
    }
  }

  SECTION("BidirectionalIterator")
  {
    SECTION("std::list")
    {
      base64_checks<std::list<char>>(
          decoded_two_byte_padding, encoded_two_byte_padding, tag);
      base64_checks<std::list<char>>(
          decoded_one_byte_padding, encoded_one_byte_padding, tag);
      base64_checks<std::list<char>>(
          decoded_no_padding, encoded_no_padding, tag);
    }
  }

  SECTION("ForwardIterator")
  {
    SECTION("std::forward_list")
    {
      base64_checks<std::forward_list<char>>(
          decoded_two_byte_padding, encoded_two_byte_padding, tag);
      base64_checks<std::forward_list<char>>(
          decoded_one_byte_padding, encoded_one_byte_padding, tag);
      base64_checks<std::forward_list<char>>(
          decoded_no_padding, encoded_no_padding, tag);
    }
  }

  SECTION("InputIterator")
  {
    SECTION("std::ifstream")
    {
      REQUIRE(testFilePaths.size() == 2);
      std::ifstream random_data(testFilePaths[0]);
      std::ifstream b64_random_data(testFilePaths[1]);

      stream_iterable_adapter random_input{random_data};
      stream_iterable_adapter b64_random_input{b64_random_data};

      base64_checks_impl(random_input, b64_random_input, tag);
    }
  }
}
}

TEST_CASE("b64 lazy", "[base64]")
{
  SECTION("encoding")
  {
    SECTION("common_checks")
    {
      common_checks<encode_tag>();
    }

    SECTION("Inception")
    {
      auto const text = "abcde"s;
      auto const b64_text = "YWJjZGU="s;
      auto const final_b64_text = "WVdKalpHVT0="s;

      encoder<std::string::const_iterator> first_enc(text.begin(), text.end());
      encoder<decltype(first_enc.begin())> second_enc(first_enc.begin(),
                                                      first_enc.end());

      std::string s(second_enc.begin(), second_enc.end());
      CHECK(s == final_b64_text);
    }
  }

  SECTION("decoding")
  {
    SECTION("common_checks")
    {
      common_checks<decode_tag>();
    }

    SECTION("Inception")
    {
      auto const text = "abcde"s;
      auto const b64_text = "YWJjZGU="s;
      auto const final_b64_text = "WVdKalpHVT0="s;

      decoder<std::string::const_iterator> first_dec(final_b64_text.begin(),
                                                     final_b64_text.end());
      decoder<decltype(first_dec.begin())> second_dec(first_dec.begin(),
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

      encoder<std::string::const_iterator> enc(text.begin(), text.end());
      decoder<decltype(enc.begin())> dec(enc.begin(), enc.end());

      std::string s(dec.begin(), dec.end());
      CHECK(s == text);
    }

    SECTION("encode(decode())")
    {
      auto const text = "YWJjZGU="s;

      decoder<std::string::const_iterator> dec(text.begin(), text.end());
      encoder<decltype(dec.begin())> enc(dec.begin(), dec.end());

      std::string s(enc.begin(), enc.end());
      CHECK(s == text);
    }
  }
}
