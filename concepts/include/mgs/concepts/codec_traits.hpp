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
#include <mgs/meta/concepts/iterator/input_iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/detected.hpp>

// clang-format off
//
// template <typename T,
//           typename I1 = /* see below */, typename S1 = /* see below */,
//           typename I2 = /* see below */, typename S2 = /* see below */>
// concept CodecTraits =
//   std::InputIterator<I1> &&
//   std::Sentinel<S1, I1> &&
//   std::InputIterator<I2> &&
//   std::Sentinel<S2, I2> &&
//   requires(I1 i1, S1 s1, I2 i2, S2 s2) {
//     { T::make_encoder(i1, s1) } -> TransformedInputRange;
//     { T::make_decoder(i2, s2) } -> TransformedInputRange;
//     CodecOutput<typename T::default_encoded_output, decltype(T::make_encoder(i1, s1))>;
//     CodecOutput<typename T::default_decoded_output, decltype(T::make_decoder(i2, s2))>;
//   };
//
// clang-format on

namespace mgs
{
inline namespace v1
{
namespace concepts
{
template <typename T,
          typename I1 =
              meta::detected_t<meta::result_of_begin,
                               detail::default_encoded_output_lvalue_ref<T>>,
          typename S1 =
              meta::detected_or_t<I1,
                                  meta::result_of_end,
                                  detail::default_encoded_output_lvalue_ref<T>>,
          typename I2 =
              meta::detected_t<meta::result_of_begin,
                               detail::default_decoded_output_lvalue_ref<T>>,
          typename S2 =
              meta::detected_or_t<I2,
                                  meta::result_of_end,
                                  detail::default_decoded_output_lvalue_ref<T>>>
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

  using DefaultEncodedOutput =
      meta::detected_t<detail::detected::types::default_encoded_output, T>;

  using DefaultDecodedOutput =
      meta::detected_t<detail::detected::types::default_decoded_output, T>;

  static constexpr auto const is_encoder =
      is_transformed_input_range<Encoder>::value;
  static constexpr auto const is_decoder =
      is_transformed_input_range<Decoder>::value;

  static constexpr auto const is_encoded_codec_output =
      is_codec_output<DefaultEncodedOutput, Encoder>::value;
  static constexpr auto const is_decoded_codec_output =
      is_codec_output<DefaultDecodedOutput, Encoder>::value;

public:
  using requirements =
      std::tuple<meta::concepts::iterator::is_input_iterator<I1>,
                 meta::concepts::iterator::is_sentinel<S1, I1>,
                 meta::concepts::iterator::is_input_iterator<I2>,
                 meta::concepts::iterator::is_sentinel<S2, I2>>;

  static constexpr auto const value =
      meta::concepts::iterator::is_input_iterator<I1>::value &&
      meta::concepts::iterator::is_sentinel<S1, I1>::value && is_encoder &&
      meta::concepts::iterator::is_input_iterator<I2>::value &&
      meta::concepts::iterator::is_sentinel<S2, I2>::value && is_decoder &&
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
}

template <typename T,
          typename I1 = meta::detected_t<
              meta::result_of_begin,
              concepts::detail::default_encoded_output_lvalue_ref<T>>,
          typename S1 = meta::detected_or_t<
              I1,
              meta::result_of_end,
              concepts::detail::default_encoded_output_lvalue_ref<T>>,
          typename I2 = meta::detected_t<
              meta::result_of_begin,
              concepts::detail::default_decoded_output_lvalue_ref<T>>,
          typename S2 = meta::detected_or_t<
              I2,
              meta::result_of_end,
              concepts::detail::default_decoded_output_lvalue_ref<T>>,
          typename = std::enable_if_t<
              concepts::is_codec_traits<T, I1, S1, I2, S2>::value>>
using CodecTraits = T;
}
}
