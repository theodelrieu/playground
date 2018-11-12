#include <fstream>
#include <string>
#include <vector>

#include <catch.hpp>

#include <mgs/adapters/concepts/iterable_input_adapter.hpp>
#include <mgs/codecs/base32/decoder.hpp>
#include <mgs/codecs/base32/encoder.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>
#include <mgs/base32.hpp>

#include <test_helpers/binary_to_text.hpp>
#include <test_helpers/codecs_base.hpp>

using namespace std::string_literals;
using namespace mgs::codecs;
namespace adapter_concepts = mgs::adapters::concepts;

extern std::vector<std::string> testFilePaths;

static_assert(
    adapter_concepts::is_iterable_input_adapter<base32::encoder<char*>>::value,
    "");
static_assert(adapter_concepts::is_iterable_input_adapter<
                  base32::encoder<std::list<char>::iterator>>::value,
              "");
static_assert(adapter_concepts::is_iterable_input_adapter<
                  base32::encoder<std::forward_list<char>::iterator>>::value,
              "");
static_assert(adapter_concepts::is_iterable_input_adapter<
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
        "========"s, "**DA=2=="s, "M======="s, "MFR====="s, "MFRAFA=="s, "MFRA@==="s};
    std::vector<std::string> invalid_eof{"MFA"s, "MFRGGZDFA"s};

    invalid_input_checks<base32::decoder, mgs::exceptions::invalid_input_error>(
        invalid_chars);
    invalid_input_checks<base32::decoder,
                         mgs::exceptions::unexpected_eof_error>(invalid_eof);
  }
}

TEST_CASE("base32 codec", "[base32]")
{
  using mgs::base32;

  SECTION("Regular tests")
  {
    test_helpers::run_codec_tests<std::string>(
        base32{}, "abcde"s, "MFRGGZDF"s);
  }

  SECTION("encoded_size")
  {
    CHECK(base32::encoded_size(0) == 0);
    CHECK(base32::encoded_size(1) == 8);
    CHECK(base32::encoded_size(2) == 8);
    CHECK(base32::encoded_size(3) == 8);
    CHECK(base32::encoded_size(4) == 8);
    CHECK(base32::encoded_size(5) == 8);
    CHECK(base32::encoded_size(6) == 16);
  }

  SECTION("max_decoded_size")
  {
    CHECK(base32::max_decoded_size(0) == 0);
    CHECK(base32::max_decoded_size(1) == 0);
    CHECK(base32::max_decoded_size(2) == 0);
    CHECK(base32::max_decoded_size(3) == 0);
    CHECK(base32::max_decoded_size(4) == 0);
    CHECK(base32::max_decoded_size(5) == 0);
    CHECK(base32::max_decoded_size(6) == 0);
    CHECK(base32::max_decoded_size(7) == 0);
    CHECK(base32::max_decoded_size(8) == 5);
    CHECK(base32::max_decoded_size(32) == 20);
    CHECK(base32::max_decoded_size(33) == 0);
    CHECK(base32::max_decoded_size(31) == 0);
  }
}
