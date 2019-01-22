#pragma once

#include <mgs/codecs/basic_codec.hpp>
#include <mgs/codecs/binary_to_text/detail/basic_codec_traits.hpp>
#include <mgs/codecs/binary_to_text/detail/bitshift_traits.hpp>
#include <mgs/codecs/binary_to_text/detail/encoded_size.hpp>
#include <mgs/codecs/binary_to_text/detail/max_decoded_size.hpp>
#include <mgs/codecs/binary_to_text/padding_policy.hpp>

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
