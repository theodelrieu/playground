#include <fstream>
#include <string>
#include <vector>

#include <catch.hpp>

#include <mgs/base64/decoder.hpp>
#include <mgs/base64/encoder.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>
#include <mgs/adapters/concepts/iterable_input_adapter.hpp>

#include <test_helpers/binary_to_text.hpp>

using namespace std::string_literals;
using namespace mgs;
namespace adapter_concepts = adapters::concepts;

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

TEST_CASE("b64 lazy", "[base64]")
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
