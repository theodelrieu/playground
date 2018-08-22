#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <mgs/codecs/base32hex/decoder.hpp>
#include <mgs/codecs/base32hex/encoder.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace base32hex
{
namespace detail
{
struct codec_traits
{
  template <typename Iterator, typename Sentinel>
  static auto make_encoder(Iterator begin, Sentinel end)
  {
    return base32hex::encoder<Iterator, Sentinel>(std::move(begin),
                                                  std::move(end));
  }

  template <typename Iterator, typename Sentinel>
  static auto make_decoder(Iterator begin, Sentinel end)
  {
    return base32hex::decoder<Iterator, Sentinel>(std::move(begin),
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
