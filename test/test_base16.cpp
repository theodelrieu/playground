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

#include <mgs/detail/base16/lazy_decoder.hpp>
#include <mgs/detail/base16/lazy_encoder.hpp>
#include <mgs/detail/meta/concepts/derived_from.hpp>
#include <mgs/detail/meta/concepts/iterable.hpp>
#include <mgs/detail/meta/concepts/iterable_input_transformer.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

#include "test_base_n.hpp"

using namespace std::string_literals;
using namespace mgs;

extern std::vector<std::string> testFilePaths;

template <typename Iterator, typename Sentinel = Iterator>
using b16_encoder = detail::base16_lazy_encoder<Iterator, Sentinel>;

template <typename Iterator, typename Sentinel = Iterator>
using b16_decoder = detail::base16_lazy_decoder<Iterator, Sentinel>;

static_assert(
    detail::is_iterable_input_transformer<b16_encoder<char*>>::value, "");
static_assert(detail::is_iterable_input_transformer<
                  b16_encoder<std::list<char>::iterator>>::value,
              "");
static_assert(detail::is_iterable_input_transformer<
                  b16_encoder<std::forward_list<char>::iterator>>::value,
              "");
static_assert(detail::is_iterable_input_transformer<
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

  SECTION("encoding")
  {
    using Traits = detail::base16_encode_traits;

    SECTION("common_checks")
    {
      common_checks<Traits>(decoded, encoded);
    }

    SECTION("sentinel")
    {
      sentinel_check<Traits>(decoded.back(), encoded.back());
    }

    SECTION("Inception")
    {
      inception_check<Traits>(
          decoded.back(), encoded.back(), "363636463646363236313732"s);
    }
  }

  SECTION("decoding")
  {
    using Traits = detail::base16_decode_traits;

    SECTION("common_checks")
    {
      common_checks<Traits>(encoded, decoded);
    }

    SECTION("sentinel")
    {
      sentinel_check<Traits>(encoded.back(), decoded.back());
    }

    SECTION("Inception")
    {
      inception_check<Traits>(
          "363636463646363236313732"s, encoded.back(), decoded.back());
    }
  }

  SECTION("back and forth")
  {
    using EncoderTraits = detail::base16_encode_traits;
    using DecoderTraits = detail::base16_decode_traits;

    SECTION("decode(encode())")
    {
      back_and_forth_check<EncoderTraits, DecoderTraits>(decoded.back());
    }

    SECTION("encode(decode())")
    {
      back_and_forth_check<DecoderTraits, EncoderTraits>(encoded.back());
    }
  }

  SECTION("stream")
  {
    REQUIRE(testFilePaths.size() == 2);
    std::ifstream random_data(testFilePaths[0]);
    std::ifstream b16_random_data(testFilePaths[1]);

    using EncoderTraits = detail::base16_encode_traits;
    using DecoderTraits = detail::base16_encode_traits;

    stream_check<EncoderTraits>(random_data, b16_random_data);
    stream_check<DecoderTraits>(b16_random_data, random_data);
  }
}
