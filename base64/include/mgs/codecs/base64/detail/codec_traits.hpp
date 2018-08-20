#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <mgs/codecs/base64/decoder.hpp>
#include <mgs/codecs/base64/encoder.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace base64
{
namespace detail
{
struct codec_traits
{
  template <typename Iterator, typename Sentinel>
  using encoder = base64::encoder<Iterator, Sentinel>;

  template <typename Iterator, typename Sentinel>
  using decoder = base64::decoder<Iterator, Sentinel>;

  using default_encoded_output = std::string;
  using default_decoded_output = std::vector<std::uint8_t>;
};
}
}
}
}
}
