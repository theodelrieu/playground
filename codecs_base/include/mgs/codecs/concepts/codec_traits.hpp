#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/codecs/concepts/codec_output.hpp>
#include <mgs/codecs/detected/static_member_functions/make_decoder.hpp>
#include <mgs/codecs/detected/static_member_functions/make_encoder.hpp>
#include <mgs/codecs/detected/types/default_decoded_output.hpp>
#include <mgs/codecs/detected/types/default_encoded_output.hpp>
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/sentinel.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/iterator_t.hpp>
#include <mgs/meta/sentinel_t.hpp>
#include <mgs/ranges/concepts/transformed_input_range.hpp>

// clang-format off
//
// template <typename T,
//           typename A1 = /* see below */, typename A2 = /* see below */,
//           typename I1 = /* see below */, typename I2 = /* see below */,
//           typename S1 = /* see below */, typename S2 = /* see below */>
// concept CodecTraits =
//   std::InputIterator<I1> &&
//   std::Sentinel<S1, I1> &&
//   std::InputIterator<I2> &&
//   std::Sentinel<S2, I2> &&
//   requires(I1 i1, S1 s1, I2 i2, S2 s2) {
//     { T::make_encoder(i1, s1) } -> TransformedInputRange;
//     { T::make_decoder(i2, s2) } -> TransformedInputRange;
//     CodecOutput<A1, decltype(T::make_encoder(i1, s1))>;
//     CodecOutput<A2, decltype(T::make_decoder(i2, s2))>;
//   };
//
// clang-format on

namespace mgs
{
namespace codecs
{
template <
    typename T,
    typename A1 = meta::detected_t<detected::types::default_encoded_output, T>,
    typename A2 = meta::detected_t<detected::types::default_decoded_output, T>,
    typename I1 = meta::detected_t<meta::iterator_t, A1>,
    typename I2 = meta::detected_t<meta::iterator_t, A2>,
    typename S1 = I1,
    typename S2 = I2>
struct is_codec_traits
{
private:
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

  static constexpr auto const is_encoder =
      ranges::is_transformed_input_range<Encoder>::value;
  static constexpr auto const is_decoder =
      ranges::is_transformed_input_range<Decoder>::value;

  static constexpr auto const is_encoded_codec_output =
      is_codec_output<A1, Encoder>::value;
  static constexpr auto const is_decoded_codec_output =
      is_codec_output<A2, Encoder>::value;

public:
  using requirements = std::tuple<meta::is_input_iterator<I1>,
                                  meta::is_sentinel<S1, I1>,
                                  meta::is_input_iterator<I2>,
                                  meta::is_sentinel<S2, I2>>;

  static constexpr auto const value =
      meta::is_input_iterator<I1>::value &&
      meta::is_sentinel<S1, I1>::value && is_encoder &&
      meta::is_input_iterator<I2>::value &&
      meta::is_sentinel<S2, I2>::value && is_decoder &&
      is_encoded_codec_output && is_decoded_codec_output;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not a CodecTraits");
    static_assert(is_encoder,
                  "T::make_encoder must return a TransformedInputRange");
    static_assert(is_decoder,
                  "T::make_decoder must return a TransformedInputRange");
    static_assert(is_encoded_codec_output,
                  "T::default_encoded_output is not a CodecOutput");
    static_assert(is_decoded_codec_output,
                  "T::default_decoded_output is not a CodecOutput");
    return 1;
  }
};

template <
    typename T,
    typename A1 = meta::detected_t<detected::types::default_encoded_output, T>,
    typename A2 = meta::detected_t<detected::types::default_decoded_output, T>,
    typename I1 = meta::detected_t<meta::iterator_t, A1>,
    typename I2 = meta::detected_t<meta::iterator_t, A2>,
    typename S1 = I1,
    typename S2 = I2>
constexpr auto is_codec_traits_v =
    is_codec_traits<T, A1, A2, I1, I2, S1, S2>::value;

template <
    typename T,
    typename A1 =
        meta::detected_t<detected::types::default_encoded_output, T>,
    typename A2 =
        meta::detected_t<detected::types::default_decoded_output, T>,
    typename I1 = meta::detected_t<meta::iterator_t, A1>,
    typename I2 = meta::detected_t<meta::iterator_t, A2>,
    typename S1 = I1,
    typename S2 = I2,
    typename = std::enable_if_t<
        is_codec_traits<T, A1, A2, I1, I2, S1, S2>::value>>
using CodecTraits = T;
}
}
