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

#include <mgs/detail/base32/lazy_decoder.hpp>
#include <mgs/detail/base32/lazy_encoder.hpp>
#include <mgs/detail/meta/concepts/derived_from.hpp>
#include <mgs/detail/meta/concepts/iterable.hpp>
#include <mgs/detail/meta/concepts/iterable_input_transformer.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

#include "test_base_n.hpp"

using namespace std::string_literals;
using namespace mgs;

extern std::vector<std::string> testFilePaths;

template <typename Iterator, typename Sentinel = Iterator>
using b32_encoder = detail::base32_lazy_encoder<Iterator, Sentinel>;

template <typename Iterator, typename Sentinel = Iterator>
using b32_decoder = detail::base32_lazy_decoder<Iterator, Sentinel>;

static_assert(
    detail::is_iterable_input_transformer<b32_encoder<char*>>::value, "");
static_assert(detail::is_iterable_input_transformer<
                  b32_encoder<std::list<char>::iterator>>::value,
              "");
static_assert(detail::is_iterable_input_transformer<
                  b32_encoder<std::forward_list<char>::iterator>>::value,
              "");
static_assert(detail::is_iterable_input_transformer<
                  b32_encoder<std::istreambuf_iterator<char>>>::value,
              "");

TEST_CASE("b32 lazy", "[base32]")
{
  std::vector<std::string> decoded{"a"s, "ab"s, "abc"s, "abcd"s, "abcde"s};
  std::vector<std::string> encoded{
      "ME======"s, "MFRA===="s, "MFRGG==="s, "MFRGGZA="s, "MFRGGZDF"s};

  using EncoderTraits = detail::base32_encode_traits;
  using DecoderTraits = detail::base32_decode_traits;

  SECTION("encoding")
  {
    SECTION("common_checks")
    {
      common_checks<EncoderTraits>(decoded, encoded);
    }

    SECTION("sentinel")
    {
      sentinel_check<EncoderTraits>(decoded.back(), encoded.back());
    }

    SECTION("Inception")
    {
      inception_check<EncoderTraits>(
          decoded.back(), encoded.back(), "JVDFER2HLJCEM==="s);
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
      sentinel_check<DecoderTraits>(encoded.back(), decoded.back());
    }

    SECTION("Inception")
    {
      inception_check<DecoderTraits>(
          "JVDFER2HLJCEM==="s, encoded.back(), decoded.back());
    }
  }

  SECTION("back and forth")
  {
    SECTION("decode(encode())")
    {
      back_and_forth_check<EncoderTraits, DecoderTraits>(decoded.back());
    }

    SECTION("encode(decode())")
    {
      back_and_forth_check<DecoderTraits, EncoderTraits>(encoded.back());
    }
  }

  SECTION("stream")
  {
    REQUIRE(testFilePaths.size() == 2);
    std::ifstream random_data(testFilePaths[0]);
    std::ifstream b32_random_data(testFilePaths[1]);

    stream_check<EncoderTraits>(random_data, b32_random_data);
    stream_check<DecoderTraits>(b32_random_data, random_data);
  }

  SECTION("invalid input")
  {
    // std::vector<std::string> invalid_chars{
    //     "="s, "*"s, "M======="s, "MFR====="s, "MFRAFA=="s, "MFRA@"s};
    std::vector<std::string> invalid_eof{"MFA"s, "MFRGGZDFA"s};

    invalid_input_checks<DecoderTraits, mgs::invalid_input_error>(
        {"MFR====="s});
    // invalid_input_checks<DecoderTraits, mgs::unexpected_eof_error>(invalid_eof);
  }
}
