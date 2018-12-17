#include <fstream>
#include <string>
#include <vector>

#include <catch.hpp>

#include <mgs/adapters/concepts/iterable_transformed_input_adapter.hpp>
#include <mgs/base32hex.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

#include <test_helpers/binary_to_text.hpp>
#include <test_helpers/codecs_base.hpp>

using namespace std::string_literals;
using namespace mgs;
namespace adapter_concepts = mgs::adapters::concepts;

extern std::vector<std::string> testFilePaths;

static_assert(adapter_concepts::is_iterable_transformed_input_adapter<
                  base32hex::encoder<char*>>::value,
              "");
static_assert(adapter_concepts::is_iterable_transformed_input_adapter<
                  base32hex::encoder<std::list<char>::iterator>>::value,
              "");
static_assert(adapter_concepts::is_iterable_transformed_input_adapter<
                  base32hex::encoder<std::forward_list<char>::iterator>>::value,
              "");
static_assert(adapter_concepts::is_iterable_transformed_input_adapter<
                  base32hex::encoder<std::istreambuf_iterator<char>>>::value,
              "");

TEST_CASE("base32hex", "[base32hex]")
{
  std::vector<std::string> decoded{"a"s, "ab"s, "abc"s, "abcd"s, "abcde"s};
  std::vector<std::string> encoded{
      "C4======"s, "C5H0===="s, "C5H66==="s, "C5H66P0="s, "C5H66P35"s};

  SECTION("encoding")
  {
    SECTION("common_checks")
    {
      common_checks<base32hex::encoder>(decoded, encoded);
    }

    SECTION("sentinel")
    {
      sentinel_check<base32hex::encoder>(decoded.back(), encoded.back());
    }

    SECTION("Inception")
    {
      inception_check<base32hex::encoder>(
          decoded.back(), encoded.back(), "8CQKGDHMA0PJA==="s);
    }
  }

  SECTION("decoding")
  {
    SECTION("common_checks")
    {
      common_checks<base32hex::decoder>(encoded, decoded);
    }

    SECTION("sentinel")
    {
      sentinel_check<base32hex::decoder>(encoded.back(), decoded.back());
    }

    SECTION("Inception")
    {
      inception_check<base32hex::decoder>(
          "8CQKGDHMA0PJA==="s, encoded.back(), decoded.back());
    }
  }

  SECTION("back and forth")
  {
    SECTION("decode(encode())")
    {
      back_and_forth_check<base32hex::encoder, base32hex::decoder>(
          decoded.back());
    }

    SECTION("encode(decode())")
    {
      back_and_forth_check<base32hex::decoder, base32hex::encoder>(
          encoded.back());
    }
  }

  SECTION("stream")
  {
    REQUIRE(testFilePaths.size() == 2);
    std::ifstream random_data(testFilePaths[0]);
    std::ifstream b32hex_random_data(testFilePaths[1]);

    stream_check<base32hex::encoder>(random_data, b32hex_random_data);
    stream_check<base32hex::decoder>(b32hex_random_data, random_data);
  }

  SECTION("invalid input")
  {
    std::vector<std::string> invalid_chars{"========"s,
                                           "**+====/"s,
                                           "V======="s,
                                           "ABC====="s,
                                           "ABCDEF=="s,
                                           "ABCDE@=="s};
    std::vector<std::string> invalid_eof{"ABC"s, "ABDHCAV"s};

    invalid_input_checks<base32hex::decoder, exceptions::invalid_input_error>(
        invalid_chars);
    invalid_input_checks<base32hex::decoder, exceptions::unexpected_eof_error>(
        invalid_eof);
  }

  SECTION("max_transformed_size")
  {
    SECTION("encoder")
    {
      static_assert(adapter_concepts::is_sized_transformed_input_adapter<
                        base32hex::encoder<char const*>>::value,
                    "");
      static_assert(
          !adapter_concepts::is_sized_transformed_input_adapter<
              base32hex::encoder<std::list<char>::const_iterator>>::value,
          "");

      SECTION("Small string")
      {
        auto const decoded = "abcdefghijklm"s;
        auto enc = base32hex::make_encoder(decoded.begin(), decoded.end());

        CHECK(enc.max_transformed_size() == 24);
      }

      SECTION("Huge string")
      {
        std::string huge_str(10000, 0);
        auto enc = base32hex::make_encoder(huge_str.begin(), huge_str.end());

        CHECK(enc.max_transformed_size() == 16000);
      }
    }

    SECTION("decoder")
    {
      static_assert(adapter_concepts::is_sized_transformed_input_adapter<
                        base32hex::decoder<char const*>>::value,
                    "");
      static_assert(
          !adapter_concepts::is_sized_transformed_input_adapter<
              base32hex::decoder<std::list<char>::const_iterator>>::value,
          "");

      SECTION("Small string")
      {
        auto const encoded = "C5H66P0="s;

        auto dec = base32hex::make_decoder(encoded.begin(), encoded.end());
        CHECK(dec.max_transformed_size() == 4);
        dec.seek_forward(2);
        CHECK(dec.max_transformed_size() == 2);
      }

      SECTION("Huge string")
      {
        auto const encoded = base32hex::encode<std::string>(std::string(10000, 0));

        auto dec = base32hex::make_decoder(encoded.begin(), encoded.end());
        CHECK(dec.max_transformed_size() == 10000);
      }

      SECTION("Invalid size")
      {
        auto encoded = base32hex::encode<std::string>(std::string(10000, 0));
        encoded.pop_back();

        auto dec = base32hex::make_decoder(encoded.begin(), encoded.end());
        CHECK_THROWS_AS(dec.max_transformed_size(),
                        mgs::exceptions::invalid_input_error);
      }
    }
  }
}

TEST_CASE("base32hex codec", "[base32hex]")
{
  SECTION("Regular tests")
  {
    test_helpers::run_codec_tests<std::string>(
        base32hex{}, "abcde"s, "C5H66P35"s);
  }

  SECTION("encoded_size")
  {
    CHECK(base32hex::encoded_size(0) == 0);
    CHECK(base32hex::encoded_size(1) == 8);
    CHECK(base32hex::encoded_size(2) == 8);
    CHECK(base32hex::encoded_size(3) == 8);
    CHECK(base32hex::encoded_size(4) == 8);
    CHECK(base32hex::encoded_size(5) == 8);
    CHECK(base32hex::encoded_size(6) == 16);
  }

  SECTION("max_decoded_size")
  {
    CHECK(base32hex::max_decoded_size(0) == 0);
    CHECK(base32hex::max_decoded_size(8) == 5);
    CHECK(base32hex::max_decoded_size(32) == 20);

    CHECK_THROWS_AS(base32hex::max_decoded_size(1),
                    exceptions::invalid_input_error);
    CHECK_THROWS_AS(base32hex::max_decoded_size(2),
                    exceptions::invalid_input_error);
    CHECK_THROWS_AS(base32hex::max_decoded_size(3),
                    exceptions::invalid_input_error);
    CHECK_THROWS_AS(base32hex::max_decoded_size(4),
                    exceptions::invalid_input_error);
    CHECK_THROWS_AS(base32hex::max_decoded_size(5),
                    exceptions::invalid_input_error);
    CHECK_THROWS_AS(base32hex::max_decoded_size(6),
                    exceptions::invalid_input_error);
    CHECK_THROWS_AS(base32hex::max_decoded_size(7),
                    exceptions::invalid_input_error);
    CHECK_THROWS_AS(base32hex::max_decoded_size(33),
                    exceptions::invalid_input_error);
    CHECK_THROWS_AS(base32hex::max_decoded_size(31),
                    exceptions::invalid_input_error);
  }
}
