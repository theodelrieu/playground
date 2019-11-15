#pragma once

#include <string>
#include <utility>
#include <vector>

#include <mgs/base_n/decode_algorithm.hpp>
#include <mgs/base_n/encode_algorithm.hpp>
#include <mgs/codecs/concepts/byte_type.hpp>
#include <mgs/codecs/concepts/input_source.hpp>
#include <mgs/codecs/iterator_sentinel_source.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/types/element_type.hpp>
#include <mgs/meta/iter_value_t.hpp>
#include <mgs/meta/priority_tag.hpp>

namespace mgs
{
namespace detail
{
template <typename EncodingTraits, typename DecodingTraits>
class basic_codec_traits
{
private:
  template <typename IS,
            typename = codecs::byte_type<
                meta::detected_t<meta::detected::types::element_type, IS>>>
  static base_n::encode_algorithm<EncodingTraits, IS> make_encoder_impl(
      meta::priority_tag<1>, codecs::input_source<IS> is)
  {
    return base_n::encode_algorithm<EncodingTraits, IS>(std::move(is));
  }

  template <typename... Args>
  static auto make_encoder_impl(meta::priority_tag<0>, Args&&... args)
      -> base_n::encode_algorithm<
          EncodingTraits,
          decltype(codecs::make_iterator_sentinel_source(std::forward<Args>(args)...))>
  {
    auto is = codecs::make_iterator_sentinel_source(std::forward<Args>(args)...);
    return base_n::encode_algorithm<EncodingTraits, decltype(is)>(is);
  }

  template <typename IS,
            typename = codecs::byte_type<
                meta::detected_t<meta::detected::types::element_type, IS>>>
  static base_n::decode_algorithm<DecodingTraits, IS> make_decoder_impl(
      meta::priority_tag<1>, codecs::input_source<IS> is)
  {
    return base_n::decode_algorithm<DecodingTraits, IS>(std::move(is));
  }

  template <typename... Args>
  static auto make_decoder_impl(meta::priority_tag<0>, Args&&... args)
      -> base_n::decode_algorithm<
          DecodingTraits,
          decltype(codecs::make_iterator_sentinel_source(std::forward<Args>(args)...))>
  {
    auto is = codecs::make_iterator_sentinel_source(std::forward<Args>(args)...);
    return base_n::decode_algorithm<DecodingTraits, decltype(is)>(is);
  }

public:
  template <typename... Args>
  static auto make_encoder(Args&&... args)
      -> decltype(basic_codec_traits::make_encoder_impl(
          meta::priority_tag<1>{}, std::forward<Args>(args)...))
  {
    return make_encoder_impl(meta::priority_tag<1>{},
                             std::forward<Args>(args)...);
  }

  template <typename... Args>
  static auto make_decoder(Args&&... args)
      -> decltype(basic_codec_traits::make_decoder_impl(
          meta::priority_tag<1>{}, std::forward<Args>(args)...))
  {
    return make_decoder_impl(meta::priority_tag<1>{},
                             std::forward<Args>(args)...);
  }

  using default_encoded_output = std::string;
  using default_decoded_output = std::vector<unsigned char>;
};
}
}
