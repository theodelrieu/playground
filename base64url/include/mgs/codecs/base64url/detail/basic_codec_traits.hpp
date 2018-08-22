#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace base64url
{
namespace detail
{
template <
    template <typename Iterator, typename Sentinel, typename...> class Encoder,
    template <typename Iterator, typename Sentinel, typename...> class Decoder>
struct basic_codec_traits
{
  template <typename Iterator, typename Sentinel>
  static auto make_encoder(Iterator begin, Sentinel end)
  {
    return Encoder<Iterator, Sentinel>(std::move(begin), std::move(end));
  }

  template <typename Iterator, typename Sentinel>
  static auto make_decoder(Iterator begin, Sentinel end)
  {
    return Decoder<Iterator, Sentinel>(std::move(begin), std::move(end));
  }

  using default_encoded_output = std::string;
  using default_decoded_output = std::vector<std::uint8_t>;
};
}
}
}
}
}
