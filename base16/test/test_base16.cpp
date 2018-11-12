#include <fstream>
#include <string>
#include <vector>

#include <catch.hpp>

#include <mgs/adapters/concepts/iterable_input_adapter.hpp>
#include <mgs/base16.hpp>
#include <mgs/codecs/base16/decoder.hpp>
#include <mgs/codecs/base16/encoder.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

#include <test_helpers/binary_to_text.hpp>
#include <test_helpers/codecs_base.hpp>

using namespace std::string_literals;
using namespace mgs::codecs;
namespace adapter_concepts = mgs::adapters::concepts;

extern std::vector<std::string> testFilePaths;

static_assert(
    adapter_concepts::is_iterable_input_adapter<base16::encoder<char*>>::value,
    "");
static_assert(adapter_concepts::is_iterable_input_adapter<
                  base16::encoder<std::list<char>::iterator>>::value,
              "");
static_assert(adapter_concepts::is_iterable_input_adapter<
                  base16::encoder<std::forward_list<char>::iterator>>::value,
              "");
static_assert(adapter_concepts::is_iterable_input_adapter<
                  base16::encoder<std::istreambuf_iterator<char>>>::value,
              "");

TEST_CASE("base16 lazy", "[base16]")
{
  std::vector<std::string> decoded{
      "f"s, "fo"s, "foo"s, "foob"s, "fooba"s, "foobar"s};
  std::vector<std::string> encoded{
      "66"s,
      "666F"s,
      "666F6F"s,
      "666F6F62"s,
      "666F6F6261"s,
      "666F6F626172"s,
  };

  SECTION("encoding")
  {
    SECTION("common_checks")
    {
      common_checks<base16::encoder>(decoded, encoded);
    }

    SECTION("sentinel")
    {
      sentinel_check<base16::encoder>(decoded.back(), encoded.back());
    }

    SECTION("Inception")
    {
      inception_check<base16::encoder>(
          decoded.back(), encoded.back(), "363636463646363236313732"s);
    }
  }

  SECTION("decoding")
  {
    SECTION("common_checks")
    {
      SECTION("case sensitive")
      {
        common_checks<base16::decoder>(encoded, decoded);
      }

      SECTION("case insensitive")
      {
        std::vector<std::string> encoded_lower{
            "66"s,
            "666f"s,
            "666f6f"s,
            "666f6f62"s,
            "666f6f6261"s,
            "666f6f626172"s,
        };

        common_checks<base16::decoder>(encoded_lower, decoded);
      }
    }

    SECTION("sentinel")
    {
      sentinel_check<base16::decoder>(encoded.back(), decoded.back());
    }

    SECTION("Inception")
    {
      inception_check<base16::decoder>(
          "363636463646363236313732"s, encoded.back(), decoded.back());
    }
  }

  SECTION("back and forth")
  {
    SECTION("decode(encode())")
    {
      back_and_forth_check<base16::encoder, base16::decoder>(decoded.back());
    }

    SECTION("encode(decode())")
    {
      back_and_forth_check<base16::decoder, base16::encoder>(encoded.back());
    }
  }

  SECTION("stream")
  {
    REQUIRE(testFilePaths.size() == 2);
    std::ifstream random_data(testFilePaths[0]);
    std::ifstream b16_random_data(testFilePaths[1]);

    stream_check<base16::encoder>(random_data, b16_random_data);
    stream_check<base16::decoder>(b16_random_data, random_data);
  }

  SECTION("invalid input")
  {
    std::vector<std::string> invalid_chars{"=="s, "**"s, "0G"s};
    std::vector<std::string> invalid_eof{"0F0"s};

    invalid_input_checks<base16::decoder, mgs::exceptions::invalid_input_error>(
        invalid_chars);
    invalid_input_checks<base16::decoder,
                         mgs::exceptions::unexpected_eof_error>(invalid_eof);
  }
}

TEST_CASE("base16 codec", "[base16]")
{
  using mgs::base16;

  SECTION("Regular tests")
  {
    test_helpers::run_codec_tests<std::string>(
        base16{}, "foobar"s, "666F6F626172"s);
  }

  SECTION("encoded_size")
  {
    CHECK(base16::encoded_size(0) == 0);
    CHECK(base16::encoded_size(1) == 2);
    CHECK(base16::encoded_size(2) == 4);
    CHECK(base16::encoded_size(3) == 6);
    CHECK(base16::encoded_size(4) == 8);
    CHECK(base16::encoded_size(5) == 10);
    CHECK(base16::encoded_size(8) == 16);
  }

  SECTION("max_decoded_size")
  {
    CHECK(base16::max_decoded_size(0) == 0);
    CHECK(base16::max_decoded_size(2) == 1);
    CHECK(base16::max_decoded_size(4) == 2);
    CHECK(base16::max_decoded_size(32) == 16);

    CHECK_THROWS_AS(base16::max_decoded_size(1),
                    mgs::exceptions::invalid_input_error);
    CHECK_THROWS_AS(base16::max_decoded_size(3),
                    mgs::exceptions::invalid_input_error);
    CHECK_THROWS_AS(base16::max_decoded_size(33),
                    mgs::exceptions::invalid_input_error);
  }
}
