#pragma once

#include <string>
#include <utility>
#include <vector>

#include <mgs/codecs/binary_to_text/basic_decoder.hpp>
#include <mgs/codecs/binary_to_text/basic_encoder.hpp>
#include <mgs/concepts/byte.hpp>
#include <mgs/meta/iter_value_t.hpp>
#include <mgs/ranges/basic_transformed_input_range.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace binary_to_text
{
namespace detail
{
template <typename EncodingTraits, typename DecodingTraits>
struct basic_codec_traits
{
  template <
      typename Iterator,
      typename Sentinel,
      typename = mgs::Byte<meta::detected_t<meta::iter_value_t, Iterator>>>
  static auto make_encoder(Iterator begin, Sentinel end)
  {
    return ranges::basic_transformed_input_range<
        basic_encoder<Iterator, Sentinel, EncodingTraits>>(std::move(begin),
                                                           std::move(end));
  }

  template <
      typename Iterator,
      typename Sentinel,
      typename = mgs::Byte<meta::detected_t<meta::iter_value_t, Iterator>>>
  static auto make_decoder(Iterator begin, Sentinel end)
  {
    return ranges::basic_transformed_input_range<
        basic_decoder<Iterator, Sentinel, DecodingTraits>>(std::move(begin),
                                                           std::move(end));
  }

  using default_encoded_output = std::string;
  using default_decoded_output = std::vector<unsigned char>;
};
}
}
}
}
}
