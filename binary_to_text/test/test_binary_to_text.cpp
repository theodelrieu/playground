#include <algorithm>
#include <chrono>
#include <deque>
#include <forward_list>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <type_traits>
#include <vector>

#include <catch.hpp>

#include <mgs/codecs/binary_to_text/basic_codec.hpp>
#include <mgs/codecs/binary_to_text/basic_decoder.hpp>
#include <mgs/codecs/binary_to_text/basic_encoder.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>
#include <mgs/meta/concepts/core/derived_from.hpp>

#include <test_helpers/binary_to_text.hpp>

using namespace std::string_literals;
using namespace mgs;
using namespace mgs::codecs;

namespace
{
struct base2_encoding_traits
{
  using alphabet_t = char const[2];

  static constexpr alphabet_t alphabet = {'0', '1'};

  static constexpr auto const padding_policy =
      binary_to_text::padding_policy::none;

  static int index_of(char c)
  {
    auto const begin = std::begin(alphabet);
    auto const end = std::end(alphabet);
    auto it = std::find(begin, end, c);
    if (it == end)
      return -1;
    return std::distance(begin, it);
  }
};

constexpr base2_encoding_traits::alphabet_t base2_encoding_traits::alphabet;

template <typename Iterator, typename Sentinel = Iterator>
using base2_encoder = adapters::transformer_adapter<
    binary_to_text::basic_encoder<Iterator, Sentinel, base2_encoding_traits>>;

template <typename Iterator, typename Sentinel = Iterator>
using base2_decoder = adapters::transformer_adapter<
    binary_to_text::basic_decoder<Iterator, Sentinel, base2_encoding_traits>>;

struct base2_codec_traits
{
  template <typename Iterator, typename Sentinel>
  static auto make_encoder(Iterator begin, Sentinel end)
  {
    return base2_encoder<Iterator, Sentinel>(std::move(begin), std::move(end));
  }

  template <typename Iterator, typename Sentinel>
  static auto make_decoder(Iterator begin, Sentinel end)
  {
    return base2_decoder<Iterator, Sentinel>(std::move(begin), std::move(end));
  }

  using default_encoded_output = std::string;
  using default_decoded_output = std::vector<std::uint8_t>;
};
}

TEST_CASE("base2", "[binary_to_text]")
{
  std::vector<std::string> decoded{"abcd"s, "abcde"s, "abcdef"s};
  std::vector<std::string> encoded{
      "01100001011000100110001101100100"s,
      "0110000101100010011000110110010001100101"s,
      "011000010110001001100011011001000110010101100110"s};

  SECTION("encoding")
  {
    SECTION("common_checks")
    {
      common_checks<base2_encoder>(decoded, encoded);
    }

    SECTION("sentinel")
    {
      sentinel_check<base2_encoder>(decoded.back(), encoded.back());
    }

    SECTION("Inception")
    {
      inception_check<base2_encoder>(
          "a"s,
          "01100001"s,
          "0011000000110001001100010011000000110000001100000011000000110001"s);
    }
  }

  SECTION("decoding")
  {
    SECTION("common_checks")
    {
      common_checks<base2_decoder>(encoded, decoded);
    }

    SECTION("sentinel")
    {
      sentinel_check<base2_decoder>(encoded.back(), decoded.back());
    }

    SECTION("Inception")
    {
      inception_check<base2_decoder>(
          "0011000000110001001100010011000000110000001100000011000000110001"s,
          "01100001"s,
          "a"s);
    }
  }

  SECTION("back and forth")
  {
    SECTION("decode(encode())")
    {
      back_and_forth_check<base2_encoder, base2_decoder>("a"s);
    }

    SECTION("encode(decode())")
    {
      back_and_forth_check<base2_decoder, base2_encoder>(
          "0011000000110001001100010011000000110000001100000011000000110001"s);
    }
  }

  SECTION("invalid input")
  {
    std::vector<std::string> invalid_chars{"***====="s, "23423423"s};
    std::vector<std::string> invalid_eof{"1010101"s, "101010101"s};

    invalid_input_checks<base2_decoder, mgs::exceptions::invalid_input_error>(
        invalid_chars);
    invalid_input_checks<base2_decoder, mgs::exceptions::unexpected_eof_error>(
        invalid_eof);
  }

  SECTION("basic_codec")
  {
    using base2 = binary_to_text::basic_codec<base2_codec_traits>;

    char const tab[5] = "abcd";

    auto const encoded = base2::encode(tab);
    CHECK(encoded == "01100001011000100110001101100100"s);
    char const tab2[33] = "01100001011000100110001101100100";
    auto const decoded = base2::decode<std::string>(tab2);
    CHECK(decoded == tab);
  }

}
