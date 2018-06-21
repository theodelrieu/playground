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

#include <mgs/detail/base64/lazy_encoder.hpp>
#include <mgs/detail/base64/lazy_decoder.hpp>
#include <mgs/detail/meta/concepts/derived_from.hpp>
#include <mgs/detail/meta/concepts/iterable.hpp>
#include <mgs/detail/meta/concepts/iterable_input_transformer.hpp>
#include <mgs/exceptions/parse_error.hpp>

using namespace std::string_literals;
using namespace mgs;

template <typename Iterator, typename Sentinel = Iterator>
using b64_encoder = detail::base64_lazy_encoder<Iterator, Sentinel>;

template <typename Iterator, typename Sentinel = Iterator>
using b64_decoder = detail::base64_lazy_decoder<Iterator, Sentinel>;

static_assert(
    detail::is_iterable_input_transformer<b64_encoder<char*>>::value, "");
static_assert(detail::is_iterable_input_transformer<
                  b64_encoder<std::list<char>::iterator>>::value,
              "");
static_assert(detail::is_iterable_input_transformer<
                  b64_encoder<std::forward_list<char>::iterator>>::value,
              "");
static_assert(detail::is_iterable_input_transformer<
                  b64_encoder<std::istreambuf_iterator<char>>>::value,
              "");

TEST_CASE("b64 lazy", "[base64]")
{
  std::vector<std::string> decoded{"abcd"s, "abcde"s, "abcdef"s};
  std::vector<std::string> encoded{"YWJjZA=="s, "YWJjZGU="s, "YWJjZGVm"s};

  SECTION("encoding")
  {
    using Traits = detail::base64_encode_traits;

    SECTION("common_checks")
    {
      common_checks<Traits>(decoded, encoded);
    }

    SECTION("sentinel")
    {
      sentinel_check<Traits>("abcde"s, "YWJjZGU="s);
    }

    SECTION("Inception")
    {
      inception_check("abcde"s, "YWJjZGU="s, "WVdKalpHVT0="s);
    }
  }

  SECTION("decoding")
  {
    using Traits = detail::base64_decode_traits;

    SECTION("common_checks")
    {
      common_checks<Traits>(encoded, decoded);
    }

    SECTION("sentinel")
    {
      sentinel_check<Traits>("YWJjZGU="s, "abcde"s);
    }

    SECTION("Inception")
    {
      inception_check("WVdKalpHVT0="s, "YWJjZGU="s, "abcde"s);
    }
  }

  SECTION("back and forth")
  {
    using EncoderTraits = detail::base64_encode_traits;
    using DecoderTraits = detail::base64_decode_traits;

    SECTION("decode(encode())")
    {
      back_and_forth_check<EncoderTraits, DecoderTraits>("abcde"s);
    }

    SECTION("encode(decode())")
    {
      back_and_forth_check<DecoderTraits, EncoderTraits>("YWJjZGU="s);
    }
  }
// TODO do streams
  SECTION("stream")
  {
    REQUIRE(testFilePaths.size() == 2);
    std::ifstream random_data(testFilePaths[0]);
    std::ifstream b64_random_data(testFilePaths[1]);

    stream_iterable_adapter random_input{random_data};
    stream_iterable_adapter b64_random_input{b64_random_data};

    using EncoderTraits = detail::base64_encode_traits;
    using DecoderTraits = detail::base64_encode_traits;

    base_n_checks_impl<EncoderTraits>(random_input, b64_random_input);
    base_n_checks_impl<DecoderTraits>(b64_random_input, random_input);
  }
}

TEST_CASE("b32 lazy", "[base32]")
{
  // TODO same test layout than b64
  std::vector<std::string> decoded{"a"s, "ab"s, "abc"s, "abcd"s, "abcde"s};
  std::vector<std::string> encoded{
      "ME======"s, "MFRA===="s, "MFRGG==="s, "MFRGGZA="s, "MFRGGZDF"s};

  using EncoderTraits = detail::base32_encode_traits;
  using DecoderTraits = detail::base32_decode_traits;

  SECTION("common_checks")
  {
    SECTION("encode")
    {
      common_checks<EncoderTraits>(decoded, encoded);
    }

    SECTION("decode")
    {
      common_checks<DecoderTraits>(encoded, decoded);
    }
  }
}

TEST_CASE("b16 lazy", "[base16]")
{
  // TODO same test layout than b64
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

  using EncoderTraits = detail::base16_encode_traits;
  using DecoderTraits = detail::base16_decode_traits;

  SECTION("common_checks")
  {
    SECTION("encode")
    {
      common_checks<EncoderTraits>(decoded, encoded);
    }

    SECTION("decode")
    {
      common_checks<DecoderTraits>(encoded, decoded);
    }
  }
}
