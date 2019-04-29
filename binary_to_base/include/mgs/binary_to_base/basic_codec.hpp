#pragma once

#include <mgs/binary_to_base/detail/basic_codec_traits.hpp>
#include <mgs/binary_to_base/detail/bitshift_traits.hpp>
#include <mgs/binary_to_base/detail/encoded_size.hpp>
#include <mgs/binary_to_base/detail/max_decoded_size.hpp>
#include <mgs/binary_to_base/padding_policy.hpp>
#include <mgs/codecs/basic_codec.hpp>
#include <mgs/meta/ssize_t.hpp>

namespace mgs
{
namespace binary_to_base
{
inline namespace v1
{
template <typename EncodingTraits, typename DecodingTraits = EncodingTraits>
class basic_codec
  : public codecs::basic_codec<
        detail::basic_codec_traits<EncodingTraits, DecodingTraits>>
{
  using base = codecs::basic_codec<
      detail::basic_codec_traits<EncodingTraits, DecodingTraits>>;

public:
  using base::decode;
  using base::encode;
  using base::make_decoder;
  using base::make_encoder;

  static constexpr meta::ssize_t encoded_size(meta::ssize_t decoded_size)
  {
    return detail::encoded_size<EncodingTraits>{}(decoded_size);
  }

  static constexpr meta::ssize_t max_decoded_size(meta::ssize_t encoded_size)
  {
    return detail::max_decoded_size<DecodingTraits>{}(encoded_size);
  }
};
}
}
}
