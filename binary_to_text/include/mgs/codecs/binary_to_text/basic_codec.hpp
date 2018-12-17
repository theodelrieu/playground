#pragma once

#include <mgs/codecs/basic_codec.hpp>

#include <mgs/codecs/binary_to_text/detail/basic_codec_traits.hpp>
#include <mgs/codecs/binary_to_text/detail/bitshift_traits.hpp>
#include <mgs/codecs/binary_to_text/detail/encoded_size.hpp>
#include <mgs/codecs/binary_to_text/detail/max_decoded_size.hpp>
#include <mgs/codecs/binary_to_text/padding_policy.hpp>
#include <mgs/codecs/concepts/codec_output.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace binary_to_text
{
template <typename EncodingTraits, typename DecodingTraits = EncodingTraits>
class basic_codec
  : public codecs::basic_codec<
        detail::basic_codec_traits<EncodingTraits, DecodingTraits>>
{
  using base = codecs::basic_codec<
      detail::basic_codec_traits<EncodingTraits, DecodingTraits>>;

public:
  using base::encode;
  using base::decode;
  using base::make_encoder;
  using base::make_decoder;

  template <typename T = typename base::default_encoded_output,
            std::size_t N = 0>
  static codecs::concepts::
      CodecOutput<T, typename base::template encoder<char const*>>
      encode(char const (&tab)[N])
  {
    auto const end_it = std::find(std::begin(tab), std::end(tab), '\0');
    return base::template encode<T>(std::begin(tab), end_it);
  }

  template <typename T = typename base::default_decoded_output,
            std::size_t N = 0>

  static codecs::concepts::
      CodecOutput<T, typename base::template decoder<char const*>>
      decode(char const (&tab)[N])
  {
    auto const end_it = std::find(std::begin(tab), std::end(tab), '\0');
    return base::template decode<T>(std::begin(tab), end_it);
  }

  static constexpr std::size_t encoded_size(std::size_t decoded_size)
  {
    return detail::encoded_size<EncodingTraits>{}(decoded_size);
  }

  static constexpr std::size_t max_decoded_size(std::size_t encoded_size)
  {
    return detail::max_decoded_size<DecodingTraits>{}(encoded_size);
  }
};
}
}
}
}
