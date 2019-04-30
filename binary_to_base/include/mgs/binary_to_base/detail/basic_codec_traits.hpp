#pragma once

#include <string>
#include <utility>
#include <vector>

#include <mgs/binary_to_base/basic_decoder.hpp>
#include <mgs/binary_to_base/basic_encoder.hpp>
#include <mgs/codecs/concepts/byte.hpp>
#include <mgs/meta/iter_value_t.hpp>
#include <mgs/ranges/basic_transformed_input_range.hpp>

namespace mgs
{
namespace detail
{
template <typename EncodingTraits, typename DecodingTraits>
struct basic_codec_traits
{
  template <typename Iterator,
            typename Sentinel,
            typename = mgs::codecs::Byte<meta::iter_value_t<Iterator>>>
  static auto make_encoder(Iterator begin, Sentinel end)
  {
    return ranges::basic_transformed_input_range<
        binary_to_base::basic_encoder<Iterator, Sentinel, EncodingTraits>>(
        std::move(begin), std::move(end));
  }

  template <typename Iterator,
            typename Sentinel,
            typename = mgs::codecs::Byte<meta::iter_value_t<Iterator>>>
  static auto make_decoder(Iterator begin, Sentinel end)
  {
    return ranges::basic_transformed_input_range<
        binary_to_base::basic_decoder<Iterator, Sentinel, DecodingTraits>>(
        std::move(begin), std::move(end));
  }

  using default_encoded_output = std::string;
  using default_decoded_output = std::vector<unsigned char>;
};
}
}