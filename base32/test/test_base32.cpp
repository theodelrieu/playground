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

#include <mgs/detail/base32/decoder.hpp>
#include <mgs/detail/base32/encoder.hpp>
#include <mgs/meta/concepts/derived_from.hpp>
#include <mgs/meta/concepts/iterable.hpp>
#include <mgs/meta/concepts/iterable_input_adapter.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

#include <test_helpers/base_n.hpp>

using namespace std::string_literals;
using namespace mgs;

extern std::vector<std::string> testFilePaths;

template <typename Iterator, typename Sentinel = Iterator>
using b32_encoder = detail::base32_encoder<Iterator, Sentinel>;

template <typename Iterator, typename Sentinel = Iterator>
using b32_decoder = detail::base32_decoder<Iterator, Sentinel>;

static_assert(meta::is_iterable_input_adapter<b32_encoder<char*>>::value, "");
static_assert(meta::is_iterable_input_adapter<
                  b32_encoder<std::list<char>::iterator>>::value,
              "");
static_assert(meta::is_iterable_input_adapter<
                  b32_encoder<std::forward_list<char>::iterator>>::value,
              "");
static_assert(meta::is_iterable_input_adapter<
                  b32_encoder<std::istreambuf_iterator<char>>>::value,
              "");

TEST_CASE("b32 lazy", "[base32]")
{
  std::vector<std::string> decoded{"a"s, "ab"s, "abc"s, "abcd"s, "abcde"s};
  std::vector<std::string> encoded{
      "ME======"s, "MFRA===="s, "MFRGG==="s, "MFRGGZA="s, "MFRGGZDF"s};

  using Encoder = detail::base_n_encoder<detail::base32_encode_traits>;
  using Decoder = detail::base_n_decoder<detail::base32_decode_traits>;

  SECTION("encoding")
  {
    SECTION("common_checks")
    {
      common_checks<Encoder>(decoded, encoded);
    }

    SECTION("sentinel")
    {
      sentinel_check<Encoder>(decoded.back(), encoded.back());
    }

    SECTION("Inception")
    {
      inception_check<Encoder>(
          decoded.back(), encoded.back(), "JVDFER2HLJCEM==="s);
    }
  }

  SECTION("decoding")
  {
    SECTION("common_checks")
    {
      common_checks<Decoder>(encoded, decoded);
    }

    SECTION("sentinel")
    {
      sentinel_check<Decoder>(encoded.back(), decoded.back());
    }

    SECTION("Inception")
    {
      inception_check<Decoder>(
          "JVDFER2HLJCEM==="s, encoded.back(), decoded.back());
    }
  }

  SECTION("back and forth")
  {
    SECTION("decode(encode())")
    {
      back_and_forth_check<Encoder, Decoder>(decoded.back());
    }

    SECTION("encode(decode())")
    {
      back_and_forth_check<Decoder, Encoder>(encoded.back());
    }
  }

  SECTION("stream")
  {
    REQUIRE(testFilePaths.size() == 2);
    std::ifstream random_data(testFilePaths[0]);
    std::ifstream b32_random_data(testFilePaths[1]);

    stream_check<Encoder>(random_data, b32_random_data);
    stream_check<Decoder>(b32_random_data, random_data);
  }

  SECTION("invalid input")
  {
    std::vector<std::string> invalid_chars{
        "="s, "*"s, "M======="s, "MFR====="s, "MFRAFA=="s, "MFRA@"s};
    std::vector<std::string> invalid_eof{"MFA"s, "MFRGGZDFA"s};

    invalid_input_checks<Decoder, mgs::invalid_input_error>(invalid_chars);
    invalid_input_checks<Decoder, mgs::unexpected_eof_error>(invalid_eof);
  }
}

TEST_CASE("base32hex", "[base32]")
{
  std::vector<std::string> decoded{"a"s, "ab"s, "abc"s, "abcd"s, "abcde"s};
  std::vector<std::string> encoded{
      "C4======"s, "C5H0===="s, "C5H66==="s, "C5H66P0="s, "C5H66P35"s};

  using Encoder = detail::base_n_encoder<detail::base32hex_encode_traits>;
  using Decoder = detail::base_n_decoder<detail::base32hex_decode_traits>;

  SECTION("encoding")
  {
    SECTION("common_checks")
    {
      common_checks<Encoder>(decoded, encoded);
    }
  }

  SECTION("decoding")
  {
    SECTION("common_checks")
    {
      common_checks<Decoder>(encoded, decoded);
    }
  }
}
