#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include <catch.hpp>

#include <mgs/adapters/concepts/iterable_transformed_input_adapter.hpp>
#include <mgs/adapters/concepts/sized_transformed_input_adapter.hpp>
#include <mgs/base64.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

#include <test_helpers/binary_to_text.hpp>
#include <test_helpers/codecs_base.hpp>

using namespace std::string_literals;
using namespace mgs;
namespace adapter_concepts = adapters::concepts;

extern std::vector<std::string> testFilePaths;

static_assert(
    adapter_concepts::is_iterable_transformed_input_adapter<base64::encoder<char*>>::value,
    "");
static_assert(adapter_concepts::is_iterable_transformed_input_adapter<
                  base64::encoder<std::list<char>::iterator>>::value,
              "");
static_assert(adapter_concepts::is_iterable_transformed_input_adapter<
                  base64::encoder<std::forward_list<char>::iterator>>::value,
              "");
static_assert(adapter_concepts::is_iterable_transformed_input_adapter<
                  base64::encoder<std::istreambuf_iterator<char>>>::value,
              "");

TEST_CASE("base64 low level", "[base64]")
{
  std::vector<std::string> decoded{"abcd"s, "abcde"s, "abcdef"s};
  std::vector<std::string> encoded{"YWJjZA=="s, "YWJjZGU="s, "YWJjZGVm"s};

  SECTION("encoding")
  {
    SECTION("common_checks")
    {
      common_checks<base64::encoder>(decoded, encoded);
    }

    SECTION("sentinel")
    {
      sentinel_check<base64::encoder>("abcde"s, "YWJjZGU="s);
    }

    SECTION("Inception")
    {
      inception_check<base64::encoder>("abcde"s, "YWJjZGU="s, "WVdKalpHVT0="s);
    }
  }

  SECTION("decoding")
  {
    SECTION("common_checks")
    {
      common_checks<base64::decoder>(encoded, decoded);
    }

    SECTION("sentinel")
    {
      sentinel_check<base64::decoder>("YWJjZGU="s, "abcde"s);
    }

    SECTION("Inception")
    {
      inception_check<base64::decoder>("WVdKalpHVT0="s, "YWJjZGU="s, "abcde"s);
    }
  }

  SECTION("back and forth")
  {
    SECTION("decode(encode())")
    {
      back_and_forth_check<base64::encoder, base64::decoder>("abcde"s);
    }

    SECTION("encode(decode())")
    {
      back_and_forth_check<base64::decoder, base64::encoder>("YWJjZGU="s);
    }
  }

  SECTION("stream")
  {
    REQUIRE(testFilePaths.size() == 2);

    SECTION("huge files")
    {
      std::ifstream random_data(testFilePaths[0]);
      std::ifstream b64_random_data(testFilePaths[1]);

      stream_check<base64::encoder>(random_data, b64_random_data);
      stream_check<base64::decoder>(b64_random_data, random_data);
    }

    SECTION("small buffers")
    {
      std::string clear("abcde");
      std::string encoded("YWJjZGU=");
      std::stringstream ss(clear);
      std::stringstream ss2(encoded);

      CHECK(base64::encode(ss) == encoded);
      CHECK(base64::decode<std::string>(ss2) == clear);
    }
  }

  SECTION("invalid input")
  {
    std::vector<std::string> invalid_chars{
        "===="s, "*AAA"s, "Y==="s, "ZA==YWJj"s, "YW=j"s, "ZA===AAA"s, "ZAW@"s};
    std::vector<std::string> invalid_eof{"YWJ"s, "YWJjZ"s};

    invalid_input_checks<base64::decoder, exceptions::invalid_input_error>(
        invalid_chars);
    invalid_input_checks<base64::decoder, exceptions::unexpected_eof_error>(
        invalid_eof);
  }

  SECTION("max_transformed_size")
  {
    SECTION("encoder")
    {
      static_assert(adapter_concepts::is_sized_transformed_input_adapter<
                        base64::encoder<char const*>>::value,
                    "");
      static_assert(
          !adapter_concepts::is_sized_transformed_input_adapter<
              base64::encoder<std::list<char>::const_iterator>>::value,
          "");

      SECTION("Small string")
      {
        auto const decoded = "abcdefghijklm"s;
        auto enc = base64::make_encoder(decoded.begin(), decoded.end());

        CHECK(enc.max_transformed_size() == 20);
      }

      SECTION("Huge string")
      {
        std::string huge_str(10000, 0);
        auto enc = base64::make_encoder(huge_str.begin(), huge_str.end());

        CHECK(enc.max_transformed_size() == 13336);
      }
    }

    SECTION("decoder")
    {
      static_assert(adapter_concepts::is_sized_transformed_input_adapter<
                        base64::decoder<char const*>>::value,
                    "");
      static_assert(
          !adapter_concepts::is_sized_transformed_input_adapter<
              base64::decoder<std::list<char>::const_iterator>>::value,
          "");

      SECTION("Small string")
      {
        auto const encoded = "WVdKalpHVT0="s;
        
        auto dec = base64::make_decoder(encoded.begin(), encoded.end());
        CHECK(dec.max_transformed_size() == 8);
        dec.seek_forward(5);
        CHECK(dec.max_transformed_size() == 3);
      }

      SECTION("Huge string")
      {
        auto const encoded = base64::encode<std::string>(std::string(10000, 0));

        auto dec = base64::make_decoder(encoded.begin(), encoded.end());
        CHECK(dec.max_transformed_size() == 10002);

        // trigger last decode operation, padding is removed, exact size is
        // returned
        dec.seek_forward(9984);
        CHECK(dec.max_transformed_size() == 16);
      }

      SECTION("Invalid size")
      {
        auto encoded = base64::encode<std::string>(std::string(10000, 0));
        encoded.pop_back();

        auto dec = base64::make_decoder(encoded.begin(), encoded.end());
        CHECK_THROWS_AS(dec.max_transformed_size(),
                        mgs::exceptions::invalid_input_error);
      }
    }
  }
}

TEST_CASE("base64 codec", "[base64]")
{
  SECTION("Regular tests")
  {
    test_helpers::run_basic_codec_tests<base64>("abcde"s, "YWJjZGU="s);
  }

  SECTION("encoded_size")
  {
    CHECK(base64::encoded_size(0) == 0);
    CHECK(base64::encoded_size(1) == 4);
    CHECK(base64::encoded_size(2) == 4);
    CHECK(base64::encoded_size(3) == 4);
    CHECK(base64::encoded_size(4) == 8);
    CHECK(base64::encoded_size(5) == 8);
    CHECK(base64::encoded_size(6) == 8);
  }

  SECTION("max_decoded_size")
  {
    CHECK(base64::max_decoded_size(0) == 0);
    CHECK(base64::max_decoded_size(4) == 3);
    CHECK(base64::max_decoded_size(8) == 6);
    CHECK(base64::max_decoded_size(32) == 24);

    CHECK_THROWS_AS(base64::max_decoded_size(1),
                    exceptions::invalid_input_error);
    CHECK_THROWS_AS(base64::max_decoded_size(2),
                    exceptions::invalid_input_error);
    CHECK_THROWS_AS(base64::max_decoded_size(3),
                    exceptions::invalid_input_error);
    CHECK_THROWS_AS(base64::max_decoded_size(5),
                    exceptions::invalid_input_error);
    CHECK_THROWS_AS(base64::max_decoded_size(6),
                    exceptions::invalid_input_error);
    CHECK_THROWS_AS(base64::max_decoded_size(7),
                    exceptions::invalid_input_error);
    CHECK_THROWS_AS(base64::max_decoded_size(33),
                    exceptions::invalid_input_error);
    CHECK_THROWS_AS(base64::max_decoded_size(31),
                    exceptions::invalid_input_error);
  }
}
