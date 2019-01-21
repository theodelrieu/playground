#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include <catch.hpp>

#include <mgs/ranges/concepts/transformed_input_range.hpp>
#include <mgs/ranges/concepts/sized_transformed_input_range.hpp>
#include <mgs/base64.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

#include <test_helpers/codec_helpers.hpp>
#include <test_helpers/noop_iterator.hpp>

using namespace std::string_literals;
using namespace mgs;
namespace range_concepts = ranges::concepts;

extern std::vector<std::string> testFilePaths;

static_assert(
    range_concepts::is_transformed_input_range<base64::encoder<char*>>::value,
    "");
static_assert(range_concepts::is_transformed_input_range<
                  base64::encoder<std::list<char>::iterator>>::value,
              "");
static_assert(range_concepts::is_transformed_input_range<
                  base64::encoder<std::forward_list<char>::iterator>>::value,
              "");
static_assert(range_concepts::is_transformed_input_range<
                  base64::encoder<std::istreambuf_iterator<char>>>::value,
              "");

TEST_CASE("base64", "[base64]")
{
  std::vector<std::string> decoded{"abcd"s, "abcde"s, "abcdef"s};
  std::vector<std::string> encoded{"YWJjZA=="s, "YWJjZGU="s, "YWJjZGVm"s};
  auto const encoded_twice = "WVdKalpHVm0="s;

  SECTION("Common tests")
  {
    for (auto i = 0; i < encoded.size(); ++i)
    {
      test_helpers::basic_codec_tests<base64>(decoded[i], encoded[i]);
      test_helpers::test_std_containers<base64>(decoded[i], encoded[i]);
      test_helpers::test_input_streams<base64>(decoded[i], encoded[i]);
      test_helpers::test_back_and_forth<base64>(decoded[i], encoded[i]);
    }

    test_helpers::test_encode_twice<base64>(decoded.back(), encoded_twice);
  }

  SECTION("stream")
  {
    REQUIRE(testFilePaths.size() == 2);
    std::ifstream random_data(testFilePaths[0]);
    std::ifstream b64_random_data(testFilePaths[1]);

    using iterator = std::istreambuf_iterator<char>;

    auto encoder = base64::make_encoder(iterator(random_data), iterator());
    test_helpers::check_equal(
        encoder.begin(), encoder.end(), iterator(b64_random_data), iterator());

    random_data.seekg(0);
    b64_random_data.seekg(0);

    auto decoder = base64::make_decoder(iterator(b64_random_data), iterator());
    test_helpers::check_equal(
        decoder.begin(), decoder.end(), iterator{random_data}, iterator());
  }

  SECTION("invalid input")
  {
    std::vector<std::string> invalid_chars{
        "===="s, "*AAA"s, "Y==="s, "ZA==YWJj"s, "YW=j"s, "ZA===AAA"s, "ZAW@"s};
    std::vector<std::string> invalid_eof{"YWJ"s, "YWJjZ"s};

    for (auto const& elem : invalid_chars)
    {
      CHECK_THROWS_AS(base64::decode(elem),
                      mgs::exceptions::invalid_input_error);
    }

    for (auto const& elem : invalid_eof)
    {
      CHECK_THROWS_AS(base64::decode(elem),
                      mgs::exceptions::unexpected_eof_error);
    }
  }

  SECTION("max_transformed_size")
  {
    SECTION("encoder")
    {
      static_assert(range_concepts::is_sized_transformed_input_range<
                        base64::encoder<char const*>>::value,
                    "");
      static_assert(
          !range_concepts::is_sized_transformed_input_range<
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
      static_assert(range_concepts::is_sized_transformed_input_range<
                        base64::decoder<char const*>>::value,
                    "");
      static_assert(
          !range_concepts::is_sized_transformed_input_range<
              base64::decoder<std::list<char>::const_iterator>>::value,
          "");

      SECTION("Small string")
      {
        auto const encoded = "WVdKalpHVT0="s;
        
        auto dec = base64::make_decoder(encoded.begin(), encoded.end());
        CHECK(dec.max_transformed_size() == 8);
        dec.read(test_helpers::noop_iterator{}, 5);
        CHECK(dec.max_transformed_size() == 3);
      }

      SECTION("Huge string")
      {
        auto const encoded = base64::encode<std::string>(std::string(10000, 0));

        auto dec = base64::make_decoder(encoded.begin(), encoded.end());
        CHECK(dec.max_transformed_size() == 10002);

        // trigger last decode operation, padding is removed, exact size is
        // returned
        dec.read(test_helpers::noop_iterator{}, 9984);
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
