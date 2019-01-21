#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/codecs/concepts/codec_output.hpp>
#include <mgs/codecs/detail/detected/static_member_functions/make_decoder.hpp>
#include <mgs/codecs/detail/detected/static_member_functions/make_encoder.hpp>
#include <mgs/codecs/detail/detected/types/default_decoded_output.hpp>
#include <mgs/codecs/detail/detected/types/default_encoded_output.hpp>
#include <mgs/concepts/transformed_input_range.hpp>
#include <mgs/meta/concepts/iterator/iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/detected.hpp>

// clang-format off
//
// template <typename T, Iterator I, Sentinel<I> S>
// concept CodecTraits = requires(T t, I i, S s) {
//   { T::make_encoder(i, s) } -> TransformedInputRange;
//   { T::make_decoder(i, s) } -> TransformedInputRange;
//   CodecOutput<typename T::default_encoded_output, decltype(T::make_encoder(i, s))>;
//   CodecOutput<typename T::default_decoded_output, decltype(T::make_decoder(i, s))>;
// };
//
// clang-format on

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace concepts
{
template <typename T, typename I, typename S>
struct is_codec_traits
{
private:
  using Encoder =
      meta::detected_t<detail::detected::static_member_functions::make_encoder,
                       T,
                       I,
                       S>;
  using Decoder =
      meta::detected_t<detail::detected::static_member_functions::make_decoder,
                       T,
                       I,
                       S>;

  using DefaultEncodedOutput =
      meta::detected_t<detail::detected::types::default_encoded_output, T>;

  using DefaultDecodedOutput =
      meta::detected_t<detail::detected::types::default_decoded_output, T>;

  static constexpr auto const is_encoder =
      mgs::concepts::is_transformed_input_range<Encoder>::value;
  static constexpr auto const is_decoder =
      mgs::concepts::is_transformed_input_range<Decoder>::value;

  static constexpr auto const is_encoded_codec_output =
      is_codec_output<DefaultEncodedOutput, Encoder>::value;
  static constexpr auto const is_decoded_codec_output =
      is_codec_output<DefaultDecodedOutput, Encoder>::value;

public:
  using requirements = std::tuple<meta::concepts::iterator::is_iterator<I>,
                                  meta::concepts::iterator::is_sentinel<S, I>>;

  static constexpr auto const value =
      meta::concepts::iterator::is_iterator<I>::value &&
      meta::concepts::iterator::is_sentinel<S, I>::value && is_encoder &&
      is_decoder && is_encoded_codec_output && is_decoded_codec_output;

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
          typename I,
          typename S,
          typename = std::enable_if_t<is_codec_traits<T, I, S>::value>>
using CodecTraits = T;
}
}
}
}
