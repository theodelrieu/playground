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

#include <mgs/base_n/basic_decoder.hpp>
#include <mgs/base_n/basic_encoder.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>
#include <mgs/meta/concepts/derived_from.hpp>
#include <mgs/meta/concepts/iterable_input_adapter.hpp>

#include <test_helpers/base_n.hpp>

using namespace std::string_literals;
using namespace mgs;

namespace
{
struct base2_encoding_traits
{
  using alphabet_t = char const[2];

  static constexpr alphabet_t alphabet = {'0', '1'};

  static constexpr char const encoding_name[] = "base2";
  static constexpr auto const padding_policy = base_n::padding_policy::none;

  static auto find_char(char c)
  {
    return std::find(std::begin(alphabet), std::end(alphabet), c);
  }
};

constexpr base2_encoding_traits::alphabet_t base2_encoding_traits::alphabet;
constexpr char const base2_encoding_traits::encoding_name[];

template <typename Iterator, typename Sentinel = Iterator>
using base2_encoder =
    adapters::transformer_adapter<base_n::basic_encoder<base2_encoding_traits>,
                                  Iterator,
                                  Sentinel>;

template <typename Iterator, typename Sentinel = Iterator>
using base2_decoder =
    adapters::transformer_adapter<base_n::basic_decoder<base2_encoding_traits>,
                                  Iterator,
                                  Sentinel>;
}

TEST_CASE("base2", "[base_n]")
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
    std::vector<std::string> invalid_chars{"*"s, "2"s};
    std::vector<std::string> invalid_eof{"1010101"s, "101010101"s};

    invalid_input_checks<base2_decoder, mgs::exceptions::invalid_input_error>(
        invalid_chars);
    invalid_input_checks<base2_decoder, mgs::exceptions::unexpected_eof_error>(
        invalid_eof);
  }
}
