#pragma once

#include <string>
#include <utility>
#include <vector>

#include <mgs/base_n/basic_decoder.hpp>
#include <mgs/base_n/basic_encoder.hpp>
#include <mgs/codecs/concepts/byte_type.hpp>
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
            typename = mgs::codecs::byte_type<meta::iter_value_t<Iterator>>>
  static auto make_encoder(Iterator begin, Sentinel end)
  {
    return ranges::basic_transformed_input_range<
        base_n::basic_encoder<Iterator, Sentinel, EncodingTraits>>(
        std::move(begin), std::move(end));
  }

  template <typename Iterator,
            typename Sentinel,
            typename = mgs::codecs::byte_type<meta::iter_value_t<Iterator>>>
  static auto make_decoder(Iterator begin, Sentinel end)
  {
    return ranges::basic_transformed_input_range<
        base_n::basic_decoder<Iterator, Sentinel, DecodingTraits>>(
        std::move(begin), std::move(end));
  }

  using default_encoded_output = std::string;
  using default_decoded_output = std::vector<unsigned char>;
};
}
}
