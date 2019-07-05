#include <fstream>
#include <string>
#include <vector>

#include <catch2/catch.hpp>

#include <mgs/base32.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>
#include <mgs/ranges/concepts/transformed_input_range.hpp>

#include <test_helpers/codec_helpers.hpp>
#include <test_helpers/noop_iterator.hpp>

using namespace std::string_literals;
using namespace mgs;

extern std::vector<std::string> testFilePaths;

static_assert(ranges::is_transformed_input_range<base32::encoder<char*>>::value,
              "");
static_assert(ranges::is_transformed_input_range<
                  base32::encoder<std::list<char>::iterator>>::value,
              "");
static_assert(ranges::is_transformed_input_range<
                  base32::encoder<std::forward_list<char>::iterator>>::value,
              "");
static_assert(ranges::is_transformed_input_range<
                  base32::encoder<std::istreambuf_iterator<char>>>::value,
              "");

TEST_CASE("base32", "[base32]")
{
  std::vector<std::string> decoded{"a"s, "ab"s, "abc"s, "abcd"s, "abcde"s};
  std::vector<std::string> encoded{
      "ME======"s, "MFRA===="s, "MFRGG==="s, "MFRGGZA="s, "MFRGGZDF"s};

  auto const encoded_twice = "JVDFER2HLJCEM==="s;

  SECTION("Common tests")
  {
    for (auto i = 0u; i < encoded.size(); ++i)
    {
      test_helpers::basic_codec_tests<base32>(decoded[i], encoded[i]);
      test_helpers::test_std_containers<base32>(decoded[i], encoded[i]);
      test_helpers::test_input_streams<base32>(decoded[i], encoded[i]);
      test_helpers::test_back_and_forth<base32>(decoded[i], encoded[i]);
    }

    test_helpers::test_encode_twice<base32>(decoded.back(), encoded_twice);
  }

  SECTION("stream")
  {
    REQUIRE(testFilePaths.size() == 2);
    std::ifstream random_data(testFilePaths[0], std::ios::binary | std::ios::in);
    std::ifstream b32_random_data(testFilePaths[1], std::ios::binary | std::ios::in);

    using iterator = std::istreambuf_iterator<char>;

    auto encoder = base32::make_encoder(iterator(random_data), iterator());
    test_helpers::check_equal(
        encoder.begin(), encoder.end(), iterator(b32_random_data), iterator());

    random_data.seekg(0);
    b32_random_data.seekg(0);

    auto decoder = base32::make_decoder(iterator(b32_random_data), iterator());
    test_helpers::check_equal(
        decoder.begin(), decoder.end(), iterator{random_data}, iterator());
  }

  SECTION("invalid input")
  {
    std::vector<std::string> invalid_chars{"========"s,
                                           "**DA=2=="s,
                                           "M======="s,
                                           "MFR====="s,
                                           "MFRAFA=="s,
                                           "MFRA@==="s};
    std::vector<std::string> invalid_eof{"MFA"s, "MFRGGZDFA"s};

    for (auto const& elem : invalid_chars)
    {
      CHECK_THROWS_AS(base32::decode(elem),
                      mgs::exceptions::invalid_input_error);
    }

    for (auto const& elem : invalid_eof)
    {
      CHECK_THROWS_AS(base32::decode(elem),
                      mgs::exceptions::unexpected_eof_error);
    }
  }

  SECTION("max_transformed_size")
  {
    SECTION("encoder")
    {
      static_assert(ranges::is_sized_transformed_input_range<
                        base32::encoder<char const*>>::value,
                    "");
      static_assert(
          !ranges::is_sized_transformed_input_range<
              base32::encoder<std::list<char>::const_iterator>>::value,
          "");

      SECTION("Small string")
      {
        auto const decoded = "abcdefghijklm"s;
        auto enc = base32::make_encoder(decoded.begin(), decoded.end());

        CHECK(enc.max_transformed_size() == 24);
      }

      SECTION("Huge string")
      {
        std::string huge_str(10000, 0);
        auto enc = base32::make_encoder(huge_str.begin(), huge_str.end());

        CHECK(enc.max_transformed_size() == 16000);
      }
    }

    SECTION("decoder")
    {
      static_assert(ranges::is_sized_transformed_input_range<
                        base32::decoder<char const*>>::value,
                    "");
      static_assert(
          !ranges::is_sized_transformed_input_range<
              base32::decoder<std::list<char>::const_iterator>>::value,
          "");

      SECTION("Small string")
      {
        auto const encoded = "MFRGGZA="s;

        auto dec = base32::make_decoder(encoded.begin(), encoded.end());
        CHECK(dec.max_transformed_size() == 5);
        dec.read(test_helpers::noop_iterator{}, 2);
        CHECK(dec.max_transformed_size() == 2);
      }

      SECTION("Huge string")
      {
        auto const encoded = base32::encode<std::string>(std::string(10000, 0));

        auto dec = base32::make_decoder(encoded.begin(), encoded.end());
        CHECK(dec.max_transformed_size() == 10000);
      }

      SECTION("Invalid size")
      {
        auto encoded = base32::encode<std::string>(std::string(10000, 0));
        encoded.pop_back();

        auto dec = base32::make_decoder(encoded.begin(), encoded.end());
        CHECK(dec.max_transformed_size() == -1);
      }
    }
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
    CHECK(base32::max_decoded_size(8) == 5);
    CHECK(base32::max_decoded_size(32) == 20);

    CHECK(base32::max_decoded_size(1) == -1);
    CHECK(base32::max_decoded_size(2) == -1);
    CHECK(base32::max_decoded_size(3) == -1);
    CHECK(base32::max_decoded_size(4) == -1);
    CHECK(base32::max_decoded_size(5) == -1);
    CHECK(base32::max_decoded_size(6) == -1);
    CHECK(base32::max_decoded_size(7) == -1);
    CHECK(base32::max_decoded_size(33) == -1);
    CHECK(base32::max_decoded_size(31) == -1);
  }
}
