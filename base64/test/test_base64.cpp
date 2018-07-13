#include <algorithm>
#include <iostream>
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

#include <mgs/detail/base64/decoder.hpp>
#include <mgs/detail/base64/encoder.hpp>
#include <mgs/detail/meta/concepts/derived_from.hpp>
#include <mgs/detail/meta/concepts/iterable.hpp>
#include <mgs/detail/meta/concepts/iterable_input_transformer.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

#include <test_helpers/base_n.hpp>

using namespace std::string_literals;
using namespace mgs;

extern std::vector<std::string> testFilePaths;

template <typename Iterator, typename Sentinel = Iterator>
using b64_encoder = detail::base64_encoder<Iterator, Sentinel>;

template <typename Iterator, typename Sentinel = Iterator>
using b64_decoder = detail::base64_decoder<Iterator, Sentinel>;

static_assert(detail::is_iterable_input_transformer<b64_encoder<char*>>::value,
              "");
static_assert(detail::is_iterable_input_transformer<
                  b64_encoder<std::list<char>::iterator>>::value,
              "");
static_assert(detail::is_iterable_input_transformer<
                  b64_encoder<std::forward_list<char>::iterator>>::value,
              "");
static_assert(detail::is_iterable_input_transformer<
                  b64_encoder<std::istreambuf_iterator<char>>>::value,
              "");

namespace
{
template <typename = void>
struct custom_base64_alphabet
{
  using alphabet_t = char const[64];

  static constexpr alphabet_t alphabet = {
      '0', '1', '8', '7', '2', '3', '4', '9', '6', '5', 'K', 'L', 'M',
      '*', '/', '-', '_', '$', 'A', '{', '}', '~', '`', 'X', 'Y', 'Z',
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
      'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
      '%', '!', '@', '%', '^', '&', '(', ')', '=', ',', '+', '\\'};

  static constexpr char const encoding_name[] = "base64";
  static constexpr char const padding_character = '#';
  static constexpr auto const padding_policy = detail::base_n_padding_policy::required;
};

template <typename Dummy>
constexpr typename custom_base64_alphabet<Dummy>::alphabet_t
    custom_base64_alphabet<Dummy>::alphabet;

template <typename Dummy>
constexpr char const custom_base64_alphabet<Dummy>::encoding_name[];

struct custom_base64_encode_traits : custom_base64_alphabet<>
{
  using value_type = char;
  using difference_type = std::streamoff;
  using algorithm = detail::base_n_encoder<custom_base64_alphabet<>>;
};

struct custom_base64_decode_traits : custom_base64_alphabet<>
{
  using value_type = std::uint8_t;
  using difference_type = std::streamoff;
  using algorithm = detail::base_n_decoder<custom_base64_alphabet<>>;
};
}

TEST_CASE("b64 lazy", "[base64]")
{
  std::vector<std::string> decoded{"abcd"s, "abcde"s, "abcdef"s};
  std::vector<std::string> encoded{"YWJjZA=="s, "YWJjZGU="s, "YWJjZGVm"s};

  using EncoderTraits = detail::base64_encode_traits;
  using DecoderTraits = detail::base64_decode_traits;

  SECTION("encoding")
  {
    SECTION("common_checks")
    {
      common_checks<EncoderTraits>(decoded, encoded);
    }

    SECTION("sentinel")
    {
      sentinel_check<EncoderTraits>("abcde"s, "YWJjZGU="s);
    }

    SECTION("Inception")
    {
      inception_check<EncoderTraits>("abcde"s, "YWJjZGU="s, "WVdKalpHVT0="s);
    }
  }

  SECTION("decoding")
  {
    SECTION("common_checks")
    {
      common_checks<DecoderTraits>(encoded, decoded);
    }

    SECTION("sentinel")
    {
      sentinel_check<DecoderTraits>("YWJjZGU="s, "abcde"s);
    }

    SECTION("Inception")
    {
      inception_check<DecoderTraits>("WVdKalpHVT0="s, "YWJjZGU="s, "abcde"s);
    }
  }

  SECTION("back and forth")
  {
    SECTION("decode(encode())")
    {
      back_and_forth_check<EncoderTraits, DecoderTraits>("abcde"s);
    }

    SECTION("encode(decode())")
    {
      back_and_forth_check<DecoderTraits, EncoderTraits>("YWJjZGU="s);
    }
  }

  SECTION("stream")
  {
    REQUIRE(testFilePaths.size() == 2);
    std::ifstream random_data(testFilePaths[0]);
    std::ifstream b64_random_data(testFilePaths[1]);

    stream_check<EncoderTraits>(random_data, b64_random_data);
    stream_check<DecoderTraits>(b64_random_data, random_data);
  }

  SECTION("invalid input")
  {
    std::vector<std::string> invalid_chars{
        "="s, "*"s, "Y==="s, "ZA==YWJj"s, "YW=j"s, "ZA==="s, "ZAW@"s};
    std::vector<std::string> invalid_eof{"YWJ"s, "YWJjZ"s};

    invalid_input_checks<DecoderTraits, mgs::invalid_input_error>(
        invalid_chars);
    invalid_input_checks<DecoderTraits, mgs::unexpected_eof_error>(invalid_eof);
  }
}

TEST_CASE("base64url lazy", "[base64]")
{
  auto const encoded = "-_-_"s;

  detail::base64url_decoder<std::string::const_iterator> dec(encoded.begin(),
                                                             encoded.end());
  detail::base64url_encoder<decltype(dec.begin())> enc(dec.begin(), dec.end());
  std::string s(enc.begin(), enc.end());
  CHECK(encoded == s);
}

TEST_CASE("base64url_unpadded", "[base64]")
{
  std::vector<std::string> decoded{"abcd"s, "abcde"s};
  std::vector<std::string> encoded_unpadded{"YWJjZA"s, "YWJjZGU"s};
  std::vector<std::string> encoded_padded{"YWJjZA=="s, "YWJjZGU="s};

  using EncoderTraits = detail::base64url_unpadded_encode_traits;
  using DecoderTraits = detail::base64url_unpadded_decode_traits;

  SECTION("encode")
  {
    common_checks<EncoderTraits>(decoded, encoded_unpadded);
  }

  SECTION("decode")
  {
    common_checks<DecoderTraits>(encoded_padded, decoded);
  }
}

TEST_CASE("Custom base64 alphabet", "[base64]")
{
  std::vector<std::string> decoded{"abcd"s, "abcde"s, "abcdef"s};
  std::vector<std::string> encoded{"Y`5jZ0##"s, "Y`5jZ4}#"s, "Y`5jZ4~m"s};

  using EncoderTraits = custom_base64_encode_traits;
  using DecoderTraits = custom_base64_decode_traits;

  SECTION("encode")
  {
    common_checks<EncoderTraits>(decoded, encoded);
  }

  SECTION("decode")
  {
    common_checks<DecoderTraits>(encoded, decoded);
  }
}
