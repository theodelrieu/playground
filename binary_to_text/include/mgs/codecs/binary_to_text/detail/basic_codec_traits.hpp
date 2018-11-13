#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include <mgs/adapters/transformer_adapter.hpp>
#include <mgs/codecs/binary_to_text/basic_decoder.hpp>
#include <mgs/codecs/binary_to_text/basic_encoder.hpp>

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
  template <typename Iterator, typename Sentinel>
  static auto make_encoder(Iterator begin, Sentinel end)
  {
    return adapters::transformer_adapter<
        basic_encoder<Iterator, Sentinel, EncodingTraits>>(std::move(begin),
                                                           std::move(end));
  }

  template <typename Iterator, typename Sentinel>
  static auto make_decoder(Iterator begin, Sentinel end)
  {
    return adapters::transformer_adapter<
        basic_decoder<Iterator, Sentinel, DecodingTraits>>(std::move(begin),
                                                           std::move(end));
  }

  using default_encoded_output = std::string;
  using default_decoded_output = std::vector<std::uint8_t>;
};
}
}
}
}
}
