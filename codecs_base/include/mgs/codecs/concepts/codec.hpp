#pragma once

#include <type_traits>

#include <mgs/adapters/concepts/input_adapter.hpp>
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
// template <typename T, Iterable I, typename EncodedOut, typename DecodedOut = EncodedOut>
// concept Codec = requires (result_of_begin_t<I> a, result_of_end_t<I> b, I
// const& c) {
//   // lazy, so only iterators to avoid lifetime issues.
//   using Encoder = decltype(T::make_encoder(a, b));
//   using Decoder = decltype(T::make_encoder(a, b));
//
//   InputAdapter<Encoder>;
//   InputAdapter<Decoder>;
//
//   EncodedOutput<EncodedOut, T, result_of_begin_t<Encoder>>;
//   DecodedOutput<DecodedOut, T, result_of_begin_t<Decoder>>;
//   // eager, user specifies return type.
//   Same<Out, decltype(T::encode<Out>(a, b))>;
//   Same<Out, decltype(T::encode<Out>(c))>;
//   Same<Out, decltype(T::decode<Out>(a, b))>;
//   Same<Out, decltype(T::decode<Out>(c))>;
// };

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace concepts
{
template <typename T, typename Out, typename It, typename = void>
struct is_codec : std::false_type
{
};

template <typename T, typename Out, typename It>
struct is_codec<
    T,
    Out,
    It,
    std::enable_if_t<meta::concepts::iterator::is_iterable<It>::value &&
                     is_codec_output<Out, T>::value>>
{
private:
  using I = meta::result_of_begin_t<It>;
  using S = meta::result_of_end_t<It>;

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

  using EncoderIterator = meta::detected_t<meta::result_of_begin_t, Encoder>;
  using DecoderIterator = meta::detected_t<meta::result_of_begin_t, Decoder>;

public:
  static constexpr auto const value =
      adapters::concepts::is_input_adapter<Encoder>::value &&
      adapters::concepts::is_input_adapter<Decoder>::value &&
      is_codec_output<Out, Encoder>::value &&
      is_codec_output<Out, Decoder>::value &&
      std::is_same<
          Out,
          meta::detected_t<detail::detected::static_member_functions::encode,
                           T,
                           Out,
                           EncoderIterator,
                           EncoderIterator>>::value &&
      std::is_same<
          Out,
          meta::detected_t<detail::detected::static_member_functions::decode,
                           T,
                           Out,
                           DecoderIterator,
                           DecoderIterator>>::value &&
      std::is_same<
          Out,
          meta::detected_t<detail::detected::static_member_functions::encode,
                           T,
                           Out,
                           It const&>>::value &&
      std::is_same<
          Out,
          meta::detected_t<detail::detected::static_member_functions::decode,
                           T,
                           Out,
                           It const&>>::value;
};
}
}
}
}
