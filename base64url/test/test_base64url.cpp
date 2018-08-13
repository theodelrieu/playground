#include <fstream>
#include <string>
#include <vector>

#include <catch.hpp>

#include <mgs/adapters/concepts/iterable_input_adapter.hpp>
#include <mgs/base64url/decoder.hpp>
#include <mgs/base64url/encoder.hpp>
#include <mgs/base64url/nopad_decoder.hpp>
#include <mgs/base64url/nopad_encoder.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

#include <test_helpers/binary_to_text.hpp>

using namespace std::string_literals;
using namespace mgs;
namespace adapter_concepts = adapters::concepts;

extern std::vector<std::string> testFilePaths;

static_assert(adapter_concepts::is_iterable_input_adapter<
                  base64url::encoder<char*>>::value,
              "");
static_assert(adapter_concepts::is_iterable_input_adapter<
                  base64url::encoder<std::list<char>::iterator>>::value,
              "");
static_assert(adapter_concepts::is_iterable_input_adapter<
                  base64url::encoder<std::forward_list<char>::iterator>>::value,
              "");
static_assert(adapter_concepts::is_iterable_input_adapter<
                  base64url::encoder<std::istreambuf_iterator<char>>>::value,
              "");

static_assert(adapter_concepts::is_iterable_input_adapter<
                  base64url::decoder<char*>>::value,
              "");
static_assert(adapter_concepts::is_iterable_input_adapter<
                  base64url::decoder<std::list<char>::iterator>>::value,
              "");
static_assert(adapter_concepts::is_iterable_input_adapter<
                  base64url::decoder<std::forward_list<char>::iterator>>::value,
              "");
static_assert(adapter_concepts::is_iterable_input_adapter<
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
        "="s, "*"s, "Y==="s, "ZA==YWJj"s, "YW=j"s, "ZA==="s, "ZAW@"s};
    std::vector<std::string> invalid_eof{"YWJ"s, "YWJjZ"s};

    invalid_input_checks<base64url::decoder,
                         mgs::exceptions::invalid_input_error>(invalid_chars);
    invalid_input_checks<base64url::decoder,
                         mgs::exceptions::unexpected_eof_error>(invalid_eof);
  }
}

TEST_CASE("base64url_nopad", "[base64url]")
{
  std::vector<std::string> decoded{"abcd"s, "abcde"s};
  std::vector<std::string> encoded_unpadded{"YWJjZA"s, "YWJjZGU"s};
  std::vector<std::string> encoded_padded{"YWJjZA=="s, "YWJjZGU="s};

  SECTION("encode")
  {
    common_checks<base64url::nopad_encoder>(decoded, encoded_unpadded);
  }

  SECTION("decode")
  {
    common_checks<base64url::nopad_decoder>(encoded_padded, decoded);
  }
}
