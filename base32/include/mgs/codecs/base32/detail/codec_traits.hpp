#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <mgs/codecs/base32/decoder.hpp>
#include <mgs/codecs/base32/encoder.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace base32
{
namespace detail
{
struct codec_traits
{
  template <typename Iterator, typename Sentinel>
  static auto make_encoder(Iterator begin, Sentinel end)
  {
    return base32::encoder<Iterator, Sentinel>(std::move(begin),
                                               std::move(end));
  }

  template <typename Iterator, typename Sentinel>
  static auto make_decoder(Iterator begin, Sentinel end)
  {
    return base32::decoder<Iterator, Sentinel>(std::move(begin),
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
