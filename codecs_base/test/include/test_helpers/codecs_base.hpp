#pragma once

#include <algorithm>
#include <type_traits>

#include <catch.hpp>
#include <mgs/codecs/basic_codec.hpp>

namespace test_helpers
{
namespace detail
{
template <typename Iterable, typename Iterable2>
void check_equal(Iterable const& lhs, Iterable2 const& rhs)
{
  using std::begin;
  using std::end;

  CHECK(std::equal(begin(lhs), end(lhs), begin(rhs), end(rhs)));
}
}

template <typename EncodedOutput,
          typename DecodedOutput = EncodedOutput,
          typename CodecTraits = void,
          typename Iterable = void,
          typename Iterable2 = void>
void run_codec_tests(mgs::codecs::basic_codec<CodecTraits>,
                     Iterable const& decoded_input,
                     Iterable2 const& encoded_input)
{
  using Codec = mgs::codecs::basic_codec<CodecTraits>;
  using DefaultEncodedOutput = typename CodecTraits::default_encoded_output;
  using DefaultDecodedOutput = typename CodecTraits::default_decoded_output;

  using std::begin;
  using std::end;

  SECTION("encode")
  {
    SECTION("default type")
    {
      auto encoded_output = Codec::encode(decoded_input);
      static_assert(
          std::is_same<decltype(encoded_output), DefaultEncodedOutput>::value,
          "Invalid default encode output type");

      detail::check_equal(encoded_output, encoded_input);
    }

    SECTION("specified type")
    {
      auto encoded_output =
          Codec::template encode<EncodedOutput>(decoded_input);
      static_assert(
          std::is_same<decltype(encoded_output), EncodedOutput>::value,
          "Invalid encode output type");

      detail::check_equal(encoded_output, encoded_input);
    }

    SECTION("make_encoder")
    {
      auto encoder =
          Codec::make_encoder(begin(decoded_input), end(decoded_input));

      detail::check_equal(encoder, encoded_input);
    }
  }

  SECTION("decode")
  {
    SECTION("default type")
    {
      auto decoded_output = Codec::decode(encoded_input);
      static_assert(
          std::is_same<decltype(decoded_output), DefaultDecodedOutput>::value,
          "Invalid default decoded output type");

      detail::check_equal(decoded_output, decoded_input);
    }

    SECTION("specified type")
    {
      auto decoded_output =
          Codec::template decode<DecodedOutput>(encoded_input);
      static_assert(
          std::is_same<decltype(decoded_output), DecodedOutput>::value,
          "Invalid decode output type");

      detail::check_equal(decoded_output, decoded_input);
    }

    SECTION("make_decoder")
    {
      auto decoder =
          Codec::make_decoder(begin(encoded_input), end(encoded_input));

      detail::check_equal(decoder, decoded_input);
    }
  }
}
}
