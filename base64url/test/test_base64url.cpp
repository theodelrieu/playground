#include <fstream>
#include <string>
#include <vector>

#include <catch.hpp>

#include <mgs/adapters/concepts/iterable_transformed_input_adapter.hpp>
#include <mgs/base64url.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

#include <test_helpers/binary_to_text.hpp>
#include <test_helpers/codecs_base.hpp>

using namespace std::string_literals;
using namespace mgs;
namespace adapter_concepts = mgs::adapters::concepts;

extern std::vector<std::string> testFilePaths;

static_assert(adapter_concepts::is_iterable_transformed_input_adapter<
                  base64url::encoder<char*>>::value,
              "");
static_assert(adapter_concepts::is_iterable_transformed_input_adapter<
                  base64url::encoder<std::list<char>::iterator>>::value,
              "");
static_assert(adapter_concepts::is_iterable_transformed_input_adapter<
                  base64url::encoder<std::forward_list<char>::iterator>>::value,
              "");
static_assert(adapter_concepts::is_iterable_transformed_input_adapter<
                  base64url::encoder<std::istreambuf_iterator<char>>>::value,
              "");

static_assert(adapter_concepts::is_iterable_transformed_input_adapter<
                  base64url::decoder<char*>>::value,
              "");
static_assert(adapter_concepts::is_iterable_transformed_input_adapter<
                  base64url::decoder<std::list<char>::iterator>>::value,
              "");
static_assert(adapter_concepts::is_iterable_transformed_input_adapter<
                  base64url::decoder<std::forward_list<char>::iterator>>::value,
              "");
static_assert(adapter_concepts::is_iterable_transformed_input_adapter<
                  base64url::decoder<std::istreambuf_iterator<char>>>::value,
              "");

TEST_CASE("b64url lazy", "[base64url]")
{
  std::vector<std::string> decoded{"abcd"s, "abcde"s, "abcdef"s};
  std::vector<std::string> encoded{"YWJjZA=="s, "YWJjZGU="s, "YWJjZGVm"s};

  SECTION("encoding")
  {
    SECTION("common_checks")
    {
      common_checks<base64url::encoder>(decoded, encoded);
    }

    SECTION("sentinel")
    {
      sentinel_check<base64url::encoder>("abcde"s, "YWJjZGU="s);
    }

    SECTION("Inception")
    {
      inception_check<base64url::encoder>(
          "abcde"s, "YWJjZGU="s, "WVdKalpHVT0="s);
    }
  }

  SECTION("decoding")
  {
    SECTION("common_checks")
    {
      common_checks<base64url::decoder>(encoded, decoded);
    }

    SECTION("sentinel")
    {
      sentinel_check<base64url::decoder>("YWJjZGU="s, "abcde"s);
    }

    SECTION("Inception")
    {
      inception_check<base64url::decoder>(
          "WVdKalpHVT0="s, "YWJjZGU="s, "abcde"s);
    }
  }

  SECTION("back and forth")
  {
    SECTION("decode(encode())")
    {
      back_and_forth_check<base64url::encoder, base64url::decoder>("abcde"s);
    }

    SECTION("encode(decode())")
    {
      back_and_forth_check<base64url::decoder, base64url::encoder>("YWJjZGU="s);
    }
  }

  SECTION("stream")
  {
    REQUIRE(testFilePaths.size() == 2);
    std::ifstream random_data(testFilePaths[0]);
    std::ifstream b64url_random_data(testFilePaths[1]);

    stream_check<base64url::encoder>(random_data, b64url_random_data);
    stream_check<base64url::decoder>(b64url_random_data, random_data);
  }

  SECTION("invalid input")
  {
    std::vector<std::string> invalid_chars{
        "===="s, "*AAA"s, "Y==="s, "ZA==YWJj"s, "YW=j"s, "ZA===AAA"s, "ZAW@"s};

    std::vector<std::string> invalid_eof{"Y"s, "YWJjZ"s};

    invalid_input_checks<base64url::decoder, exceptions::invalid_input_error>(
        invalid_chars);

    invalid_input_checks<base64url::decoder, exceptions::unexpected_eof_error>(
        invalid_eof);

  }

  SECTION("max_transformed_size")
  {
    SECTION("encoder")
    {
      static_assert(adapter_concepts::is_sized_transformed_input_adapter<
                        base64url::encoder<char const*>>::value,
                    "");
      static_assert(
          !adapter_concepts::is_sized_transformed_input_adapter<
              base64url::encoder<std::list<char>::const_iterator>>::value,
          "");

      SECTION("Small string")
      {
        auto const decoded = "abcdefghijklm"s;
        auto enc = base64url::make_encoder(decoded.begin(), decoded.end());

        CHECK(enc.max_transformed_size() == 20);
      }

      SECTION("Huge string")
      {
        std::string huge_str(10000, 0);
        auto enc = base64url::make_encoder(huge_str.begin(), huge_str.end());

        CHECK(enc.max_transformed_size() == 13336);
      }
    }

    SECTION("decoder")
    {
      static_assert(adapter_concepts::is_sized_transformed_input_adapter<
                        base64url::decoder<char const*>>::value,
                    "");
      static_assert(
          !adapter_concepts::is_sized_transformed_input_adapter<
              base64url::decoder<std::list<char>::const_iterator>>::value,
          "");

      SECTION("Small string")
      {
        auto const encoded = "WVdKalpHVT0="s;

        auto dec = base64url::make_decoder(encoded.begin(), encoded.end());
        CHECK(dec.max_transformed_size() == 8);
        dec.seek_forward(5);
        CHECK(dec.max_transformed_size() == 3);
      }

      SECTION("Huge string")
      {
        auto const encoded =
            base64url::encode<std::string>(std::string(10000, 0));

        auto dec = base64url::make_decoder(encoded.begin(), encoded.end());
        CHECK(dec.max_transformed_size() == 10002);

        // trigger last decode operation, padding is removed, exact size is
        // returned
        dec.seek_forward(9984);
        CHECK(dec.max_transformed_size() == 16);
      }

      SECTION("Invalid size")
      {
        auto encoded = base64url::encode<std::string>(std::string(10000, 0));
        encoded.pop_back();

        auto dec = base64url::make_decoder(encoded.begin(), encoded.end());
        CHECK_THROWS_AS(dec.max_transformed_size(),
                        mgs::exceptions::invalid_input_error);
      }
    }
  }
}

TEST_CASE("base64url_nopad", "[base64url]")
{
  std::vector<std::string> decoded{"abcd"s, "abcde"s};
  std::vector<std::string> encoded_unpadded{"YWJjZA"s, "YWJjZGU"s};
  std::vector<std::string> encoded_padded{"YWJjZA=="s, "YWJjZGU="s};

  SECTION("encode")
  {
    common_checks<base64url_nopad::encoder>(decoded, encoded_unpadded);
  }

  SECTION("decode")
  {
    common_checks<base64url_nopad::decoder>(encoded_padded, decoded);
  }

  SECTION("invalid input")
  {
    std::vector<std::string> invalid_chars{
        "===="s, "*AAA"s, "Y==="s, "ZA==YWJj"s, "YW=j"s, "ZA===AAA"s, "ZAW@"s};

    std::vector<std::string> invalid_eof{"Y"s, "YWJjZ"s};
    std::vector<std::string> invalid_nopad_chars{"ZAAAA="s, "ZAW@=="s};

    invalid_input_checks<base64url_nopad::decoder,
                         exceptions::invalid_input_error>(invalid_chars);

    invalid_input_checks<base64url_nopad::decoder,
                         exceptions::invalid_input_error>(invalid_nopad_chars);
    invalid_input_checks<base64url_nopad::decoder,
                         exceptions::unexpected_eof_error>(invalid_eof);
  }

  SECTION("max_transformed_size")
  {
    SECTION("encoder")
    {
      static_assert(adapter_concepts::is_sized_transformed_input_adapter<
                        base64url_nopad::encoder<char const*>>::value,
                    "");
      static_assert(
          !adapter_concepts::is_sized_transformed_input_adapter<
              base64url_nopad::encoder<std::list<char>::const_iterator>>::value,
          "");

      SECTION("Small string")
      {
        auto const decoded = "abcdefghijklm"s;
        auto enc =
            base64url_nopad::make_encoder(decoded.begin(), decoded.end());

        CHECK(enc.max_transformed_size() == 18);
      }

      SECTION("Huge string")
      {
        std::string huge_str(10000, 0);
        auto enc =
            base64url_nopad::make_encoder(huge_str.begin(), huge_str.end());

        CHECK(enc.max_transformed_size() == 13334);
      }
    }

    SECTION("decoder")
    {
      static_assert(adapter_concepts::is_sized_transformed_input_adapter<
                        base64url_nopad::decoder<char const*>>::value,
                    "");
      static_assert(
          !adapter_concepts::is_sized_transformed_input_adapter<
              base64url_nopad::decoder<std::list<char>::const_iterator>>::value,
          "");

      SECTION("Small string")
      {
        auto const encoded = "WVdKalpHVT0="s;

        auto dec =
            base64url_nopad::make_decoder(encoded.begin(), encoded.end());
        CHECK(dec.max_transformed_size() == 8);
        dec.seek_forward(5);
        CHECK(dec.max_transformed_size() == 3);
      }

      SECTION("Huge string")
      {
        auto const encoded =
            base64url_nopad::encode<std::string>(std::string(10000, 0));

        auto dec =
            base64url_nopad::make_decoder(encoded.begin(), encoded.end());
        CHECK(dec.max_transformed_size() == 10000);

        // trigger last decode operation, padding is removed, exact size is
        // returned
        dec.seek_forward(9984);
        CHECK(dec.max_transformed_size() == 16);
      }

      SECTION("Invalid size")
      {
        auto encoded =
            base64url_nopad::encode<std::string>(std::string(10000, 0));
        encoded.pop_back();

        auto dec =
            base64url_nopad::make_decoder(encoded.begin(), encoded.end());
        CHECK_THROWS_AS(dec.max_transformed_size(),
                        mgs::exceptions::invalid_input_error);
      }
    }
  }
}

TEST_CASE("base64url codec", "[base64url]")
{
  SECTION("Regular tests")
  {
    test_helpers::run_codec_tests<std::string>(
        base64url{}, "abcde"s, "YWJjZGU="s);
  }

  SECTION("encoded_size")
  {
    CHECK(base64url::encoded_size(0) == 0);
    CHECK(base64url::encoded_size(1) == 4);
    CHECK(base64url::encoded_size(2) == 4);
    CHECK(base64url::encoded_size(3) == 4);
    CHECK(base64url::encoded_size(4) == 8);
    CHECK(base64url::encoded_size(5) == 8);
    CHECK(base64url::encoded_size(6) == 8);
  }

  SECTION("max_decoded_size")
  {
    CHECK(base64url::max_decoded_size(0) == 0);
    CHECK(base64url::max_decoded_size(4) == 3);
    CHECK(base64url::max_decoded_size(8) == 6);
    CHECK(base64url::max_decoded_size(32) == 24);

    CHECK_THROWS_AS(base64url::max_decoded_size(1),
                    exceptions::invalid_input_error);
    CHECK_THROWS_AS(base64url::max_decoded_size(2),
                    exceptions::invalid_input_error);
    CHECK_THROWS_AS(base64url::max_decoded_size(3),
                    exceptions::invalid_input_error);
    CHECK_THROWS_AS(base64url::max_decoded_size(5),
                    exceptions::invalid_input_error);
    CHECK_THROWS_AS(base64url::max_decoded_size(6),
                    exceptions::invalid_input_error);
    CHECK_THROWS_AS(base64url::max_decoded_size(7),
                    exceptions::invalid_input_error);
    CHECK_THROWS_AS(base64url::max_decoded_size(33),
                    exceptions::invalid_input_error);
    CHECK_THROWS_AS(base64url::max_decoded_size(31),
                    exceptions::invalid_input_error);
  }
}

TEST_CASE("base64url_nopad codec", "[base64url]")
{
  SECTION("Regular tests")
  {
    test_helpers::run_codec_tests<std::string>(
        base64url_nopad{}, "abcde"s, "YWJjZGU"s);
  }

  SECTION("encoded_size")
  {
    CHECK(base64url_nopad::encoded_size(0) == 0);
    CHECK(base64url_nopad::encoded_size(1) == 2);
    CHECK(base64url_nopad::encoded_size(2) == 3);
    CHECK(base64url_nopad::encoded_size(3) == 4);
    CHECK(base64url_nopad::encoded_size(4) == 6);
    CHECK(base64url_nopad::encoded_size(5) == 7);
    CHECK(base64url_nopad::encoded_size(6) == 8);
  }

  SECTION("max_decoded_size")
  {
    CHECK(base64url_nopad::max_decoded_size(0) == 0);
    CHECK(base64url_nopad::max_decoded_size(2) == 1);
    CHECK(base64url_nopad::max_decoded_size(3) == 2);
    CHECK(base64url_nopad::max_decoded_size(4) == 3);
    CHECK(base64url_nopad::max_decoded_size(6) == 4);
    CHECK(base64url_nopad::max_decoded_size(7) == 5);
    CHECK(base64url_nopad::max_decoded_size(8) == 6);
    CHECK(base64url_nopad::max_decoded_size(22) == 16);
    CHECK(base64url_nopad::max_decoded_size(24) == 18);

    CHECK_THROWS_AS(base64url_nopad::max_decoded_size(1),
                    exceptions::invalid_input_error);
    CHECK_THROWS_AS(base64url_nopad::max_decoded_size(5),
                    exceptions::invalid_input_error);
    CHECK_THROWS_AS(base64url_nopad::max_decoded_size(21),
                    exceptions::invalid_input_error);
  }
}
