#include <fstream>
#include <string>
#include <vector>

#include <catch.hpp>

#include <mgs/adapters/concepts/iterable_input_adapter.hpp>
#include <mgs/base32hex/decoder.hpp>
#include <mgs/base32hex/encoder.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

#include <test_helpers/binary_to_text.hpp>

using namespace std::string_literals;
using namespace mgs;
namespace adapter_concepts = adapters::concepts;

extern std::vector<std::string> testFilePaths;

static_assert(adapter_concepts::is_iterable_input_adapter<
                  base32hex::encoder<char*>>::value,
              "");
static_assert(adapter_concepts::is_iterable_input_adapter<
                  base32hex::encoder<std::list<char>::iterator>>::value,
              "");
static_assert(adapter_concepts::is_iterable_input_adapter<
                  base32hex::encoder<std::forward_list<char>::iterator>>::value,
              "");
static_assert(adapter_concepts::is_iterable_input_adapter<
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
    std::vector<std::string> invalid_chars{
        "="s, "*"s, "V======="s, "ABC====="s, "ABCDEF=="s, "ABCDE@"s};
    std::vector<std::string> invalid_eof{"ABC"s, "ABDHCAV"s};

    invalid_input_checks<base32hex::decoder,
                         mgs::exceptions::invalid_input_error>(invalid_chars);
    invalid_input_checks<base32hex::decoder,
                         mgs::exceptions::unexpected_eof_error>(invalid_eof);
  }
}
