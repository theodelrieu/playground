#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/concepts/codec_output.hpp>
#include <mgs/concepts/detail/default_output_types.hpp>
#include <mgs/concepts/detail/detected/static_member_functions/make_decoder.hpp>
#include <mgs/concepts/detail/detected/static_member_functions/make_encoder.hpp>
#include <mgs/concepts/detail/detected/types/default_decoded_output.hpp>
#include <mgs/concepts/detail/detected/types/default_encoded_output.hpp>
#include <mgs/concepts/transformed_input_range.hpp>
#include <mgs/meta/call_std/begin.hpp>
#include <mgs/meta/call_std/end.hpp>
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/sentinel.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/iterator_t.hpp>

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
namespace concepts
{
template <typename T,
          typename A1 = meta::detected_t<detail::default_encoded_output, T>,
          typename A2 = meta::detected_t<detail::default_decoded_output, T>,
          typename I1 = meta::detected_t<meta::iterator_t, A1>,
          typename I2 = meta::detected_t<meta::iterator_t, A2>,
          typename S1 = I1,
          typename S2 = I2>
struct is_codec_traits
{
private:
  using Encoder =
      meta::detected_t<detail::detected::static_member_functions::make_encoder,
                       T,
                       I1,
                       S1>;
  using Decoder =
      meta::detected_t<detail::detected::static_member_functions::make_decoder,
                       T,
                       I2,
                       S2>;

  static constexpr auto const is_encoder =
      is_transformed_input_range<Encoder>::value;
  static constexpr auto const is_decoder =
      is_transformed_input_range<Decoder>::value;

  static constexpr auto const is_encoded_codec_output =
      is_codec_output<A1, Encoder>::value;
  static constexpr auto const is_decoded_codec_output =
      is_codec_output<A2, Encoder>::value;

public:
  using requirements = std::tuple<meta::concepts::is_input_iterator<I1>,
                                  meta::concepts::is_sentinel<S1, I1>,
                                  meta::concepts::is_input_iterator<I2>,
                                  meta::concepts::is_sentinel<S2, I2>>;

  static constexpr auto const value =
      meta::concepts::is_input_iterator<I1>::value &&
      meta::concepts::is_sentinel<S1, I1>::value && is_encoder &&
      meta::concepts::is_input_iterator<I2>::value &&
      meta::concepts::is_sentinel<S2, I2>::value && is_decoder &&
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

template <typename T,
          typename A1 = meta::detected_t<detail::default_encoded_output, T>,
          typename A2 = meta::detected_t<detail::default_decoded_output, T>,
          typename I1 = meta::detected_t<meta::iterator_t, A1>,
          typename I2 = meta::detected_t<meta::iterator_t, A2>,
          typename S1 = I1,
          typename S2 = I2>
constexpr auto is_codec_traits_v =
    is_codec_traits<T, A1, A2, I1, I2, S1, S2>::value;
}

template <
    typename T,
    typename A1 = meta::detected_t<concepts::detail::default_encoded_output, T>,
    typename A2 = meta::detected_t<concepts::detail::default_decoded_output, T>,
    typename I1 = meta::detected_t<meta::iterator_t, A1>,
    typename I2 = meta::detected_t<meta::iterator_t, A2>,
    typename S1 = I1,
    typename S2 = I2,
    typename = std::enable_if_t<
        concepts::is_codec_traits<T, A1, A2, I1, I2, S1, S2>::value>>
using CodecTraits = T;
}
