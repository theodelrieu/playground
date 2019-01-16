#pragma once

#include <algorithm>
#include <type_traits>

#include <catch.hpp>

namespace test_helpers
{
namespace detail
{
template <typename Iterable, typename Iterable2>
void check_equal(Iterable& lhs, Iterable2& rhs)
{
  using std::begin;
  using std::end;

  CHECK(std::equal(begin(lhs), end(lhs), begin(rhs), end(rhs)));
}
}

// TODO static asserts is_codec
template <typename Codec, typename Iterable, typename CodecOutput>
void test_encode(Iterable& it, CodecOutput& expected)
{
  auto const encoded = Codec::template encode<CodecOutput>(it);
  detail::check_equal(encoded, expected);
}

template <typename Codec, typename I, typename S, typename CodecOutput>
void test_encode(I i, S s, CodecOutput& expected)
{
  auto const encoded = Codec::template encode<CodecOutput>(i, s);
  detail::check_equal(encoded, expected);
}

template <typename Codec, typename I, typename S, typename Iterable>
void test_make_encoder(I i, S s, Iterable& expected)
{
  auto encoder = Codec::make_encoder(i, s);
  detail::check_equal(encoder, expected);
}

template <typename Codec, typename I1, typename S1, typename I2, typename S2>
void test_make_encoder(I1 i1, S1 s1, I2 i2, S2 s2)
{
  auto encoder = Codec::make_encoder(i1, s1);

  using std::begin;
  using std::end;
  
  CHECK(std::equal(begin(encoder), end(encoder), i2, s2));
}

template <typename Codec, typename Iterable, typename CodecOutput>
void test_decode(Iterable& it, CodecOutput& expected)
{
  auto const decoded = Codec::template decode<CodecOutput>(it);
  detail::check_equal(decoded, expected);
}

template <typename Codec, typename I, typename S, typename CodecOutput>
void test_decode(I i, S s, CodecOutput& expected)
{
  auto const decoded = Codec::template decode<CodecOutput>(i, s);
  detail::check_equal(decoded, expected);
}

template <typename Codec, typename I, typename S, typename Iterable>
void test_make_decoder(I i, S s, Iterable& expected)
{
  auto decoder = Codec::make_decoder(i, s);
  detail::check_equal(decoder, expected);
}

template <typename Codec, typename I1, typename S1, typename I2, typename S2>
void test_make_decoder(I1 i1, S1 s1, I2 i2, S2 s2)
{
  auto decoder = Codec::make_decoder(i1, s1);

  using std::begin;
  using std::end;
  
  CHECK(std::equal(begin(decoder), end(decoder), i2, s2));
}

template <typename Codec, typename Iterable1, typename Iterable2>
void run_basic_codec_tests(Iterable1 const& decoded_input,
                           Iterable2 const& encoded_input)
{
  using DefaultEncodedOutput = typename Codec::default_encoded_output;
  using DefaultDecodedOutput = typename Codec::default_decoded_output;
  using EncodedOutput = Iterable2;
  using DecodedOutput = Iterable1;

  using std::begin;
  using std::end;

  test_encode<Codec>(decoded_input, encoded_input);
  test_make_encoder<Codec>(
      begin(decoded_input), end(decoded_input), encoded_input);

  test_decode<Codec>(encoded_input, decoded_input);
  test_make_decoder<Codec>(
      begin(encoded_input), end(encoded_input), decoded_input);

  {
    auto const encoded_output = Codec::encode(decoded_input);
    static_assert(
        std::is_same<decltype(encoded_output), DefaultEncodedOutput>::value,
        "Invalid default encode output type");

    detail::check_equal(encoded_output, encoded_input);
  }
  {
    auto const decoded_output = Codec::decode(encoded_input);
    static_assert(
        std::is_same<decltype(decoded_output), DefaultDecodedOutput>::value,
        "Invalid default decoded output type");

    detail::check_equal(decoded_output, decoded_input);
  }
}
}
