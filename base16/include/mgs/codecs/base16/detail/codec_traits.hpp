#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <mgs/codecs/base16/decoder.hpp>
#include <mgs/codecs/base16/encoder.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace base16
{
namespace detail
{
struct codec_traits
{
  template <typename Iterator, typename Sentinel>
  static auto make_encoder(Iterator begin, Sentinel end)
  {
    return base16::encoder<Iterator, Sentinel>(std::move(begin),
                                               std::move(end));
  }

  template <typename Iterator, typename Sentinel>
  static auto make_decoder(Iterator begin, Sentinel end)
  {
    return base16::decoder<Iterator, Sentinel>(std::move(begin),
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
