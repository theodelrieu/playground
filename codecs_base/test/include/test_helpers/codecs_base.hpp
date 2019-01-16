#pragma once

#include <algorithm>
#include <cstddef>
#include <deque>
#include <forward_list>
#include <list>
#include <type_traits>
#include <vector>

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

template <typename Codec,
          typename CodecOutput = typename Codec::default_encoded_output,
          typename Iterable1 = void,
          typename Iterable2 = void>
void test_encode(Iterable1& it, Iterable2& expected)
{
  auto const encoded = Codec::template encode<CodecOutput>(it);
  detail::check_equal(encoded, expected);
}

template <typename Codec,
          typename CodecOutput = typename Codec::default_encoded_output,
          typename I = void,
          typename S = void,
          typename Iterable>
void test_encode(I i, S s, Iterable& expected)
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

template <typename Codec,
          typename CodecOutput = typename Codec::default_decoded_output,
          typename Iterable1 = void,
          typename Iterable2 = void>
void test_decode(Iterable1& it, Iterable2& expected)
{
  auto const decoded = Codec::template decode<CodecOutput>(it);
  detail::check_equal(decoded, expected);
}

template <typename Codec,
          typename CodecOutput = typename Codec::default_decoded_output,
          typename I = void,
          typename S = void,
          typename Iterable = void>
void test_decode(I i, S s, Iterable& expected)
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

template <typename Codec,
          typename EncodedOutput = typename Codec::default_encoded_output,
          typename DecodedOutput = EncodedOutput,
          typename Iterable1 = void,
          typename Iterable2 = void>
void basic_codec_tests(Iterable1& decoded_input, Iterable2& encoded_input)
{
  using std::begin;
  using std::end;

  test_encode<Codec, EncodedOutput>(decoded_input, encoded_input);
  test_make_encoder<Codec>(
      begin(decoded_input), end(decoded_input), encoded_input);

  test_decode<Codec, DecodedOutput>(encoded_input, decoded_input);
  test_make_decoder<Codec>(
      begin(encoded_input), end(encoded_input), decoded_input);
}

namespace detail
{
template <typename Codec, typename Container, typename Iterable1, typename Iterable2>
void test_std_container_input(Iterable1& decoded_input, Iterable2 encoded_input)
{
  using std::begin;
  using std::end;

  Container decoded(begin(decoded_input), end(decoded_input));
  Container encoded(begin(encoded_input), end(encoded_input));

  basic_codec_tests<Codec>(decoded, encoded);
}
}

template <typename Codec, typename Iterable1, typename Iterable2>
void test_std_container_outputs(Iterable1& decoded_input,
                                Iterable2& encoded_input)
{
  basic_codec_tests<Codec, std::string>(decoded_input, encoded_input);

  basic_codec_tests<Codec, std::vector<char>>(decoded_input, encoded_input);
  basic_codec_tests<Codec, std::vector<unsigned char>>(decoded_input,
                                                       encoded_input);
  basic_codec_tests<Codec, std::vector<std::int8_t>>(decoded_input,
                                                     encoded_input);
  basic_codec_tests<Codec, std::vector<std::uint8_t>>(decoded_input,
                                                      encoded_input);

  basic_codec_tests<Codec, std::deque<char>>(decoded_input, encoded_input);
  basic_codec_tests<Codec, std::deque<unsigned char>>(decoded_input,
                                                      encoded_input);
  basic_codec_tests<Codec, std::deque<std::int8_t>>(decoded_input,
                                                    encoded_input);
  basic_codec_tests<Codec, std::deque<std::uint8_t>>(decoded_input,
                                                     encoded_input);

  basic_codec_tests<Codec, std::list<char>>(decoded_input, encoded_input);
  basic_codec_tests<Codec, std::list<unsigned char>>(decoded_input,
                                                     encoded_input);
  basic_codec_tests<Codec, std::list<std::int8_t>>(decoded_input,
                                                   encoded_input);
  basic_codec_tests<Codec, std::list<std::uint8_t>>(decoded_input,
                                                    encoded_input);
}

template <typename Codec, typename Iterable1, typename Iterable2>
void test_std_container_inputs(Iterable1& decoded_input,
                               Iterable2& encoded_input)
{
  detail::test_std_container_input<Codec, std::string>(decoded_input,
                                                       encoded_input);

  detail::test_std_container_input<Codec, std::vector<char>>(decoded_input,
                                                             encoded_input);
  detail::test_std_container_input<Codec, std::vector<unsigned char>>(
      decoded_input, encoded_input);
  detail::test_std_container_input<Codec, std::vector<std::int8_t>>(
      decoded_input, encoded_input);
  detail::test_std_container_input<Codec, std::vector<std::uint8_t>>(
      decoded_input, encoded_input);

  detail::test_std_container_input<Codec, std::list<char>>(decoded_input,
                                                           encoded_input);
  detail::test_std_container_input<Codec, std::list<unsigned char>>(
      decoded_input, encoded_input);
  detail::test_std_container_input<Codec, std::list<std::int8_t>>(
      decoded_input, encoded_input);
  detail::test_std_container_input<Codec, std::list<std::uint8_t>>(
      decoded_input, encoded_input);

  detail::test_std_container_input<Codec, std::forward_list<char>>(
      decoded_input, encoded_input);
  detail::test_std_container_input<Codec, std::forward_list<unsigned char>>(
      decoded_input, encoded_input);
  detail::test_std_container_input<Codec, std::forward_list<std::int8_t>>(
      decoded_input, encoded_input);
  detail::test_std_container_input<Codec, std::forward_list<std::uint8_t>>(
      decoded_input, encoded_input);

  detail::test_std_container_input<Codec, std::deque<char>>(decoded_input,
                                                            encoded_input);
  detail::test_std_container_input<Codec, std::deque<unsigned char>>(
      decoded_input, encoded_input);
  detail::test_std_container_input<Codec, std::deque<std::int8_t>>(
      decoded_input, encoded_input);
  detail::test_std_container_input<Codec, std::deque<std::uint8_t>>(
      decoded_input, encoded_input);
}

template <typename Codec, typename Iterable1, typename Iterable2>
void test_std_containers(Iterable1& decoded_input, Iterable2& encoded_input)
{
  test_std_container_outputs<Codec>(decoded_input, encoded_input);
  test_std_container_inputs<Codec>(decoded_input, encoded_input);
}
}
