#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/codecs/concepts/codec_output.hpp>
#include <mgs/concepts/detected/static_member_functions/decode.hpp>
#include <mgs/concepts/detected/static_member_functions/encode.hpp>
#include <mgs/concepts/detected/static_member_functions/make_decoder.hpp>
#include <mgs/concepts/detected/static_member_functions/make_encoder.hpp>
#include <mgs/concepts/detected/types/default_decoded_output.hpp>
#include <mgs/concepts/detected/types/default_encoded_output.hpp>
#include <mgs/concepts/transformed_input_range.hpp>
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/range.hpp>
#include <mgs/meta/concepts/sentinel.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/iterator_t.hpp>

// clang-format off
//
// template <typename T,
//           typename A1 = /* see below */, typename A2 = /* see below */,
//           typename R1 = /* see below */, typename R2 = /* see below */,
//           typename I1 = /* see below */, typename I2 = /* see below */,
//           typename S1 = /* see below */, typename S2 = /* see below */>
// concept Codec =
//   std::InputIterator<I1> &&
//   std::Sentinel<S1, I1> &&
//   std::InputIterator<I2> &&
//   std::Sentinel<S2, I2> &&
//   requires(A1& a1, A2& a2, I1 i1, S1 s1, I2 i2, S2 s2) {
//     { T::make_encoder(i1, s1) } -> TransformedInputRange;
//     { T::make_decoder(i2, s2) } -> TransformedInputRange;
// 
//     CodecOutput<typename T::default_encoded_output, decltype(T::make_encoder(i1, s1))>;
//     CodecOutput<typename T::default_decoded_output, decltype(T::make_decoder(i2, s2))>;
//     CodecOutput<R1, decltype(T::make_encoder(i1, s1))>;
//     CodecOutput<R2, decltype(T::make_decoder(i2, s2))>;
// 
//     { T::encode(i1, s1) } -> std::Same<typename T::default_encoded_output>;
//     { T::encode(a1) } -> std::Same<typename T::default_encoded_output>;
//     { T::encode<R1>(i1, s1) } -> std::Same<R1>;
//     { T::encode<R1>(a1) } -> std::Same<R1>;
// 
//     { T::decode(i2, s2) } -> std::Same<typename T::default_decoded_output>;
//     { T::decode(a2) } -> std::Same<typename T::default_decoded_output>;
//     { T::decode<R2>(i2, s2) } -> std::Same<R2>;
//     { T::decode<R2>(a2) } -> std::Same<R2>;
//   };
//
// clang-format on

namespace mgs
{
template <typename T,
          typename A1 = detected::types::default_decoded_output<T>,
          typename A2 = detected::types::default_encoded_output<T>,
          typename R1 = detected::types::default_encoded_output<T>,
          typename R2 = detected::types::default_decoded_output<T>,
          typename I1 = meta::detected_t<meta::iterator_t, A1>,
          typename I2 = meta::detected_t<meta::iterator_t, A2>,
          typename S1 = I1,
          typename S2 = I2>
struct is_codec
{
private:
  using DefaultEncodedOutput = detected::types::default_encoded_output<T>;
  using DefaultDecodedOutput = detected::types::default_decoded_output<T>;
  using EncodeInput = std::add_lvalue_reference_t<A1>;
  using DecodeInput = std::add_lvalue_reference_t<A2>;

  using Encoder =
      meta::detected_t<detected::static_member_functions::make_encoder,
                       T,
                       I1,
                       S1>;

  using Decoder =
      meta::detected_t<detected::static_member_functions::make_decoder,
                       T,
                       I2,
                       S2>;

  using EncodeDefaultResult =
      meta::detected_t<detected::static_member_functions::encode,
                       T,
                       EncodeInput>;

  using EncodeIteratorsDefaultResult = meta::
      detected_t<detected::static_member_functions::encode, T, I1, S1>;

  using EncodeTemplateResult =
      meta::detected_t<detected::static_member_functions::encode_tpl,
                       T,
                       R1,
                       EncodeInput>;

  using EncodeIteratorsTemplateResult =
      meta::detected_t<detected::static_member_functions::encode_tpl,
                       T,
                       R1,
                       I1,
                       S1>;

  using DecodeDefaultResult =
      meta::detected_t<detected::static_member_functions::decode,
                       T,
                       DecodeInput>;

  using DecodeIteratorsDefaultResult = meta::
      detected_t<detected::static_member_functions::decode, T, I2, S2>;

  using DecodeTemplateResult =
      meta::detected_t<detected::static_member_functions::decode_tpl,
                       T,
                       R2,
                       DecodeInput>;

  using DecodeIteratorsTemplateResult =
      meta::detected_t<detected::static_member_functions::decode_tpl,
                       T,
                       R2,
                       I2,
                       S2>;

  static constexpr auto const has_make_encoder =
      is_transformed_input_range<Encoder>::value;

  static constexpr auto const has_make_decoder =
      is_transformed_input_range<Decoder>::value;

  static constexpr auto const has_default_encoded_output =
      is_codec_output<DefaultEncodedOutput, Encoder>::value;

  static constexpr auto const has_default_decoded_output =
      is_codec_output<DefaultDecodedOutput, Decoder>::value;

  static constexpr auto const has_template_encoded_output =
      is_codec_output<R1, Encoder>::value;

  static constexpr auto const has_template_decoded_output =
      is_codec_output<R2, Decoder>::value;

  static constexpr auto const has_encode_iterators_default_result =
      !std::is_same<EncodeIteratorsDefaultResult, meta::nonesuch>::value &&
      std::is_same<EncodeIteratorsDefaultResult, DefaultEncodedOutput>::value;

  static constexpr auto const has_encode_default_result =
      !std::is_same<EncodeDefaultResult, meta::nonesuch>::value &&
      std::is_same<EncodeDefaultResult, DefaultEncodedOutput>::value;

  static constexpr auto const has_encode_iterators_template_result =
      !std::is_same<EncodeIteratorsTemplateResult, meta::nonesuch>::value &&
      std::is_same<EncodeIteratorsTemplateResult, R1>::value;

  static constexpr auto const has_encode_template_result =
      !std::is_same<EncodeTemplateResult, meta::nonesuch>::value &&
      std::is_same<EncodeTemplateResult, R1>::value;

  static constexpr auto const has_decode_iterators_default_result =
      !std::is_same<DecodeIteratorsDefaultResult, meta::nonesuch>::value &&
      std::is_same<DecodeIteratorsDefaultResult, DefaultDecodedOutput>::value;

  static constexpr auto const has_decode_default_result =
      !std::is_same<DecodeDefaultResult, meta::nonesuch>::value &&
      std::is_same<DecodeDefaultResult, DefaultDecodedOutput>::value;

  static constexpr auto const has_decode_iterators_template_result =
      !std::is_same<DecodeIteratorsTemplateResult, meta::nonesuch>::value &&
      std::is_same<DecodeIteratorsTemplateResult, R2>::value;

  static constexpr auto const has_decode_template_result =
      !std::is_same<DecodeTemplateResult, meta::nonesuch>::value &&
      std::is_same<DecodeTemplateResult, R2>::value;

public:
  using requirements =
      std::tuple<meta::is_input_iterator<I1>,
                 meta::is_sentinel<S1, I1>,
                 meta::is_input_iterator<I2>,
                 meta::is_sentinel<S2, I2>>;

  static constexpr auto const value =
      meta::is_input_iterator<I1>::value &&
      meta::is_sentinel<S1, I1>::value &&
      meta::is_input_iterator<I2>::value &&
      meta::is_sentinel<S2, I2>::value &&
      has_make_encoder && has_make_decoder && has_default_encoded_output &&
      has_default_decoded_output && has_template_encoded_output &&
      has_template_decoded_output && has_encode_iterators_default_result &&
      has_encode_default_result && has_encode_iterators_template_result &&
      has_encode_template_result && has_decode_iterators_default_result &&
      has_decode_default_result && has_decode_iterators_template_result &&
      has_decode_template_result;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not a Codec");

    static_assert(
        has_make_encoder,
        "'T::make_encoder(I1, S1)' must return a TransformedInputRange");
    static_assert(
        has_make_decoder,
        "'T::make_decoder(I2, S2)' must return a TransformedInputRange");

    static_assert(has_default_encoded_output,
                  "'T::default_encoded_output' must be a CodecOutput<Encoder>");
    static_assert(has_default_decoded_output,
                  "'T::default_decoded_output' must be a CodecOutput<Decoder>");
    static_assert(has_template_encoded_output,
                  "'R1' must be a CodecOutput<Encoder>");
    static_assert(has_template_decoded_output,
                  "'R2' must be a CodecOutput<Decoder>");

    static_assert(
        has_encode_iterators_default_result,
        "'T::encode(I1, S1)' must return 'T::default_encoded_output'");
    static_assert(
        has_decode_iterators_default_result,
        "'T::decode(I2, S2)' must return 'T::default_decoded_output'");
    static_assert(has_encode_iterators_template_result,
                  "'T::encode<R1>(I1, S1)' must return 'R1'");
    static_assert(has_decode_iterators_template_result,
                  "'T::decode<R2>(I2, S2)' must return 'R2'");

    static_assert(has_encode_default_result,
                  "'T::encode(A1&)' must return 'T::default_encoded_output'");
    static_assert(has_decode_default_result,
                  "'T::decode(A2&)' must return 'T::default_decoded_output'");
    static_assert(has_encode_template_result,
                  "'T::encode<R1>(A1&)' must return 'R1'");
    static_assert(has_decode_template_result,
                  "'T::decode<R2>(A2&)' must return 'R2'");
    return 1;
  }
};

template <typename T,
          typename A1 = detected::types::default_decoded_output<T>,
          typename A2 = detected::types::default_encoded_output<T>,
          typename R1 = detected::types::default_encoded_output<T>,
          typename R2 = detected::types::default_decoded_output<T>,
          typename I1 = meta::detected_t<meta::iterator_t, A1>,
          typename I2 = meta::detected_t<meta::iterator_t, A2>,
          typename S1 = I1,
          typename S2 = I2>
constexpr auto is_codec_v = is_codec<T, A1, A2, R1, R2, I1, I2, S1, S2>::value;

template <typename T,
          typename A1 = detected::types::default_decoded_output<T>,
          typename A2 = detected::types::default_encoded_output<T>,
          typename R1 = detected::types::default_encoded_output<T>,
          typename R2 = detected::types::default_decoded_output<T>,
          typename I1 = meta::detected_t<meta::iterator_t, A1>,
          typename I2 = meta::detected_t<meta::iterator_t, A2>,
          typename S1 = I1,
          typename S2 = I2,
          typename = std::enable_if_t<
              is_codec<T, A1, A2, R1, R2, I1, I2, S1, S2>::value>>
using Codec = T;
}
