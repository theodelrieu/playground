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

#include <mgs/base32/decoder.hpp>
#include <mgs/base32/encoder.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>
#include <mgs/meta/concepts/iterable_input_adapter.hpp>

#include <test_helpers/base_n.hpp>

using namespace std::string_literals;
using namespace mgs;

extern std::vector<std::string> testFilePaths;

static_assert(meta::is_iterable_input_adapter<base32::encoder<char*>>::value,
              "");
static_assert(meta::is_iterable_input_adapter<
                  base32::encoder<std::list<char>::iterator>>::value,
              "");
static_assert(meta::is_iterable_input_adapter<
                  base32::encoder<std::forward_list<char>::iterator>>::value,
              "");
static_assert(meta::is_iterable_input_adapter<
                  base32::encoder<std::istreambuf_iterator<char>>>::value,
              "");

TEST_CASE("b32 lazy", "[base32]")
{
  std::vector<std::string> decoded{"a"s, "ab"s, "abc"s, "abcd"s, "abcde"s};
  std::vector<std::string> encoded{
      "ME======"s, "MFRA===="s, "MFRGG==="s, "MFRGGZA="s, "MFRGGZDF"s};

  SECTION("encoding")
  {
    SECTION("common_checks")
    {
      common_checks<base32::encoder>(decoded, encoded);
    }

    SECTION("sentinel")
    {
      sentinel_check<base32::encoder>(decoded.back(), encoded.back());
    }

    SECTION("Inception")
    {
      inception_check<base32::encoder>(
          decoded.back(), encoded.back(), "JVDFER2HLJCEM==="s);
    }
  }

  SECTION("decoding")
  {
    SECTION("common_checks")
    {
      common_checks<base32::decoder>(encoded, decoded);
    }

    SECTION("sentinel")
    {
      sentinel_check<base32::decoder>(encoded.back(), decoded.back());
    }

    SECTION("Inception")
    {
      inception_check<base32::decoder>(
          "JVDFER2HLJCEM==="s, encoded.back(), decoded.back());
    }
  }

  SECTION("back and forth")
  {
    SECTION("decode(encode())")
    {
      back_and_forth_check<base32::encoder, base32::decoder>(decoded.back());
    }

    SECTION("encode(decode())")
    {
      back_and_forth_check<base32::decoder, base32::encoder>(encoded.back());
    }
  }

  SECTION("stream")
  {
    REQUIRE(testFilePaths.size() == 2);
    std::ifstream random_data(testFilePaths[0]);
    std::ifstream b32_random_data(testFilePaths[1]);

    stream_check<base32::encoder>(random_data, b32_random_data);
    stream_check<base32::decoder>(b32_random_data, random_data);
  }

  SECTION("invalid input")
  {
    std::vector<std::string> invalid_chars{
        "="s, "*"s, "M======="s, "MFR====="s, "MFRAFA=="s, "MFRA@"s};
    std::vector<std::string> invalid_eof{"MFA"s, "MFRGGZDFA"s};

    invalid_input_checks<base32::decoder, mgs::exceptions::invalid_input_error>(
        invalid_chars);
    invalid_input_checks<base32::decoder, mgs::exceptions::unexpected_eof_error>(
        invalid_eof);
  }
}
//
// TEST_CASE("base32hex", "[base32]")
// {
//   std::vector<std::string> decoded{"a"s, "ab"s, "abc"s, "abcd"s, "abcde"s};
//   std::vector<std::string> encoded{
//       "C4======"s, "C5H0===="s, "C5H66==="s, "C5H66P0="s, "C5H66P35"s};
//
//   using Encoder = base_n::basic_encoder<detail::base32hex_encode_traits>;
//   using Decoder = base_n::basic_decoder<detail::base32hex_decode_traits>;
//
//   SECTION("encoding")
//   {
//     SECTION("common_checks")
//     {
//       common_checks<Encoder>(decoded, encoded);
//     }
//   }
//
//   SECTION("decoding")
//   {
//     SECTION("common_checks")
//     {
//       common_checks<Decoder>(encoded, decoded);
//     }
//   }
// }
