#pragma once

#include <type_traits>

#include <mgs/adapters/concepts/iterable_input_adapter.hpp>
#include <mgs/codecs/detail/detected/static_member_functions/create.hpp>
#include <mgs/codecs/detail/detected/static_member_functions/make_decoder.hpp>
#include <mgs/codecs/detail/detected/static_member_functions/make_encoder.hpp>
#include <mgs/codecs/detail/detected/types/output_traits.hpp>
#include <mgs/meta/call_std/begin.hpp>
#include <mgs/meta/call_std/end.hpp>
#include <mgs/meta/detected.hpp>

// Note that Codec is a typename, not a concept.
// This is because CodecOutput is a requirement of Codec concept,
// so we have to redeclare some Codec requirements here as well.

// template <typename T, typename Codec>
// concept CodecOutput = requires (Codec const& codec) {
//   TODO finish concept comment
//   Same<T, decltype(output_traits<Codec, T>::create(begin(adapter), end(adapter)))>;
// };

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace concepts
{
template <typename T, typename Codec, typename = void>
struct is_codec_output : std::false_type
{
};

template <typename T, typename Codec>
struct is_codec_output<
    T,
    Codec,
    std::enable_if_t<
        adapters::concepts::is_iterable_input_adapter<meta::detected_t<
            detail::detected::static_member_functions::make_encoder,
            Codec,
            char const*,
            char const*>>::value &&
        adapters::concepts::is_iterable_input_adapter<meta::detected_t<
            detail::detected::static_member_functions::make_decoder,
            Codec,
            char const*,
            char const*>>::value>>
{
private:
  using output_traits_t =
      meta::detected_t<detail::detected::types::output_traits, Codec, T>;

  using encoder_t = detail::detected::static_member_functions::
      make_encoder<Codec, char const*, char const*>;
  using decoder_t = detail::detected::static_member_functions::
      make_decoder<Codec, char const*, char const*>;

  using encode_output_t =
      meta::detected_t<detail::detected::static_member_functions::create,
                       output_traits_t,
                       meta::result_of_begin_t<encoder_t const&>,
                       meta::result_of_end_t<encoder_t const&>>;

  using decode_output_t =
      meta::detected_t<detail::detected::static_member_functions::create,
                       output_traits_t,
                       meta::result_of_begin_t<decoder_t const&>,
                       meta::result_of_end_t<decoder_t const&>>;

public:
  static constexpr auto const value =
      std::is_same<encode_output_t, decode_output_t>::value &&
      std::is_same<encode_output_t, T>::value;
};
}
}
}
}
