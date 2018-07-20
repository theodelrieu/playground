#include <algorithm>
#include <chrono>
#include <deque>
#include <forward_list>
#include <fstream>
#include <iterator>
#include <list>
#include <sstream>
#include <type_traits>
#include <vector>

#include <catch.hpp>

#include <mgs/detail/base16/decoder.hpp>
#include <mgs/detail/base16/encoder.hpp>
#include <mgs/meta/concepts/derived_from.hpp>
#include <mgs/meta/concepts/iterable.hpp>
#include <mgs/meta/concepts/iterable_input_adapter.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

#include <test_helpers/base_n.hpp>

using namespace std::string_literals;
using namespace mgs;

extern std::vector<std::string> testFilePaths;

template <typename Iterator, typename Sentinel = Iterator>
using b16_encoder = detail::base16_encoder<Iterator, Sentinel>;

template <typename Iterator, typename Sentinel = Iterator>
using b16_decoder = detail::base16_decoder<Iterator, Sentinel>;

static_assert(meta::is_iterable_input_adapter<b16_encoder<char*>>::value, "");
static_assert(meta::is_iterable_input_adapter<
                  b16_encoder<std::list<char>::iterator>>::value,
              "");
static_assert(meta::is_iterable_input_adapter<
                  b16_encoder<std::forward_list<char>::iterator>>::value,
              "");
static_assert(meta::is_iterable_input_adapter<
                  b16_encoder<std::istreambuf_iterator<char>>>::value,
              "");

TEST_CASE("b16 lazy", "[base16]")
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

  using Encoder = base_n::basic_encoder<detail::base16_encode_traits>;
  using Decoder = base_n::basic_decoder<detail::base16_decode_traits>;

  SECTION("encoding")
  {
    SECTION("common_checks")
    {
      common_checks<Encoder>(decoded, encoded);
    }

    SECTION("sentinel")
    {
      sentinel_check<Encoder>(decoded.back(), encoded.back());
    }

    SECTION("Inception")
    {
      inception_check<Encoder>(
          decoded.back(), encoded.back(), "363636463646363236313732"s);
    }
  }

  SECTION("decoding")
  {
    SECTION("common_checks")
    {
      SECTION("case sensitive")
      {
      common_checks<Decoder>(encoded, decoded);
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

        common_checks<Decoder>(encoded_lower, decoded);
      }
    }

    SECTION("sentinel")
    {
      sentinel_check<Decoder>(encoded.back(), decoded.back());
    }

    SECTION("Inception")
    {
      inception_check<Decoder>(
          "363636463646363236313732"s, encoded.back(), decoded.back());
    }
  }

  SECTION("back and forth")
  {
    SECTION("decode(encode())")
    {
      back_and_forth_check<Encoder, Decoder>(decoded.back());
    }

    SECTION("encode(decode())")
    {
      back_and_forth_check<Decoder, Encoder>(encoded.back());
    }
  }

  SECTION("stream")
  {
    REQUIRE(testFilePaths.size() == 2);
    std::ifstream random_data(testFilePaths[0]);
    std::ifstream b16_random_data(testFilePaths[1]);

    stream_check<Encoder>(random_data, b16_random_data);
    stream_check<Decoder>(b16_random_data, random_data);
  }

  SECTION("invalid input")
  {
    std::vector<std::string> invalid_chars{"="s, "*"s, "0G"s};
    std::vector<std::string> invalid_eof{"0F0"s};

    invalid_input_checks<Decoder, mgs::invalid_input_error>(
        invalid_chars);
    invalid_input_checks<Decoder, mgs::unexpected_eof_error>(invalid_eof);
  }
}
