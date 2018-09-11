#pragma once

#include <type_traits>

#include <mgs/adapters/concepts/iterable_input_adapter.hpp>
#include <mgs/codecs/concepts/codec_output.hpp>
#include <mgs/codecs/detail/detected/static_member_functions/decode.hpp>
#include <mgs/codecs/detail/detected/static_member_functions/encode.hpp>
#include <mgs/codecs/detail/detected/static_member_functions/make_decoder.hpp>
#include <mgs/codecs/detail/detected/static_member_functions/make_encoder.hpp>
#include <mgs/meta/call_std/begin.hpp>
#include <mgs/meta/call_std/end.hpp>
#include <mgs/meta/concepts/iterator/iterable.hpp>
#include <mgs/meta/detected.hpp>

// FIXME BinaryCodec
// template <typename T,
//           Iterable I,
//           typename EncodedOut,
//           typename DecodedOut = EncodedOut>
// concept Codec = requires (result_of_begin<I> a,
//                           result_of_end<I> b,
//                           I const& c)
// {
//   // lazy, so only iterators to avoid lifetime issues.
//   using Encoder = decltype(T::make_encoder(a, b));
//   using Decoder = decltype(T::make_encoder(a, b));
//
//   IterableInputAdapter<Encoder>;
//   IterableInputAdapter<Decoder>;
//
//   EncodedOutput<EncodedOut, result_of_begin<Encoder>>;
//   DecodedOutput<DecodedOut, result_of_begin<Decoder>>;
//
//   // eager, user specifies return type.
//   Same<EncodedOut, decltype(T::encode<EncodedOut>(a, b))>;
//   Same<EncodedOut, decltype(T::encode<EncodedOut>(c))>;
//   Same<DecodedOut, decltype(T::decode<DecodedOut>(a, b))>;
//   Same<DecodedOut, decltype(T::decode<DecodedOut>(c))>;
// };

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace concepts
{
template <typename T,
          typename Iterable,
          typename EncodedOut,
          typename DecodedOut = EncodedOut,
          typename = void>
struct is_codec : std::false_type
{
};

template <typename T,
          typename Iterable,
          typename EncodedOut,
          typename DecodedOut>
struct is_codec<
    T,
    Iterable,
    EncodedOut,
    DecodedOut,
    std::enable_if_t<meta::concepts::iterator::is_iterable<Iterable>::value>>
{
private:
  using I = meta::result_of_begin<Iterable>;
  using S = meta::result_of_end<Iterable>;

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

  using EncoderIterator = meta::detected_t<meta::result_of_begin, Encoder>;
  using DecoderIterator = meta::detected_t<meta::result_of_begin, Decoder>;

public:
  static constexpr auto const value =
      adapters::concepts::is_iterable_input_adapter<Encoder>::value &&
      adapters::concepts::is_iterable_input_adapter<Decoder>::value &&
      is_codec_output<EncodedOut, EncoderIterator>::value &&
      is_codec_output<DecodedOut, DecoderIterator>::value &&
      meta::is_detected_exact<EncodedOut,
                              detail::detected::static_member_functions::encode,
                              T,
                              EncodedOut,
                              EncoderIterator,
                              EncoderIterator>::value &&
      meta::is_detected_exact<DecodedOut,
                              detail::detected::static_member_functions::decode,
                              T,
                              DecodedOut,
                              DecoderIterator,
                              DecoderIterator>::value &&
      meta::is_detected_exact<EncodedOut,
                              detail::detected::static_member_functions::encode,
                              T,
                              EncodedOut,
                              Iterable const&>::value &&
      meta::is_detected_exact<DecodedOut,
                              detail::detected::static_member_functions::decode,
                              T,
                              DecodedOut,
                              Iterable const&>::value;
};
}
}
}
}
