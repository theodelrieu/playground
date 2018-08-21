#include <fstream>
#include <string>
#include <vector>

#include <catch.hpp>

#include <mgs/adapters/concepts/iterable_input_adapter.hpp>
#include <mgs/codecs/base64/decoder.hpp>
#include <mgs/codecs/base64/encoder.hpp>
#include <mgs/base64.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

#include <test_helpers/binary_to_text.hpp>

using namespace std::string_literals;
namespace base64 = mgs::codecs::base64;
namespace adapter_concepts = mgs::adapters::concepts;

extern std::vector<std::string> testFilePaths;

static_assert(
    adapter_concepts::is_iterable_input_adapter<base64::encoder<char*>>::value,
    "");
static_assert(adapter_concepts::is_iterable_input_adapter<
                  base64::encoder<std::list<char>::iterator>>::value,
              "");
static_assert(adapter_concepts::is_iterable_input_adapter<
                  base64::encoder<std::forward_list<char>::iterator>>::value,
              "");
static_assert(adapter_concepts::is_iterable_input_adapter<
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
    std::ifstream random_data(testFilePaths[0]);
    std::ifstream b64_random_data(testFilePaths[1]);

    stream_check<base64::encoder>(random_data, b64_random_data);
    stream_check<base64::decoder>(b64_random_data, random_data);
  }

  SECTION("invalid input")
  {
    std::vector<std::string> invalid_chars{
        "="s, "*"s, "Y==="s, "ZA==YWJj"s, "YW=j"s, "ZA==="s, "ZAW@"s};
    std::vector<std::string> invalid_eof{"YWJ"s, "YWJjZ"s};

    invalid_input_checks<base64::decoder, mgs::exceptions::invalid_input_error>(
        invalid_chars);
    invalid_input_checks<base64::decoder,
                         mgs::exceptions::unexpected_eof_error>(invalid_eof);
  }
}

TEST_CASE("base64 codec", "[base64]")
{
  SECTION("encode")
  {
    auto const input = "abcde"s;

    SECTION("default type: std::string")
    {
      auto const b64str = mgs::base64::encode(input);

      CHECK(b64str == "YWJjZGU="s);

      auto const b64vec = mgs::base64::encode<std::vector<std::uint8_t>>(input);
      CHECK(std::equal(
          b64str.begin(), b64str.end(), b64vec.begin(), b64vec.end()));
    }

    SECTION("make_encoder")
    {
      auto encoder = mgs::base64::make_encoder(input.begin(), input.end());
      std::string b64str(encoder.begin(), encoder.end());

      CHECK(b64str == "YWJjZGU="s);
    }
  }

  SECTION("decode")
  {
    auto const input = "YWJjZGU="s;

    SECTION("default type: std::vector<std::uint8_t>")
    {
      auto const decoded_str = mgs::base64::decode<std::string>(input);
      auto const decoded_vec = mgs::base64::decode(input);

      CHECK(decoded_str == "abcde"s);
      CHECK(std::equal(decoded_str.begin(),
                       decoded_str.end(),
                       decoded_vec.begin(),
                       decoded_vec.end()));
    }

    SECTION("make_decoder")
    {
      auto decoder = mgs::base64::make_decoder(input.begin(), input.end());
      std::string decoded_str(decoder.begin(), decoder.end());

      CHECK(decoded_str == "abcde"s);
    }
  }
}
