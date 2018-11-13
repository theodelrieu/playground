#pragma once

#include <bitset>
#include <cstddef>

#include <mgs/codecs/binary_to_text/detail/bitshift_traits.hpp>
#include <mgs/codecs/binary_to_text/detail/math.hpp>

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
template <typename decodingTraits>
struct output_decoder
{
  using BitshiftTraits = bitshift_traits<decodingTraits>;

  template <typename OutputIterator>
  static void decode(
      std::bitset<BitshiftTraits::nb_decoded_bits> const& decoded_bits,
      OutputIterator out,
      std::size_t n)
  {
    constexpr std::bitset<BitshiftTraits::nb_decoded_bits> mask(255);

    for (auto i = 0u; i < n; ++i)
    {
      out[i] = static_cast<std::uint8_t>(
          (decoded_bits >> (BitshiftTraits::decoded_shift - (8 * i)) & mask)
              .to_ulong());
    }
  }

  template <typename OutputIterator>
  static void decode(
      std::bitset<BitshiftTraits::nb_decoded_bits> const& decoded_bits,
      OutputIterator out)
  {
    constexpr std::bitset<BitshiftTraits::nb_decoded_bits> mask(255);

    for (auto i = 0u; i < BitshiftTraits::nb_decoded_bytes; ++i)
    {
      out[i] = static_cast<std::uint8_t>(
          (decoded_bits >> (BitshiftTraits::decoded_shift - (8 * i)) & mask)
              .to_ulong());
    }
  }
};
}
}
}
}
}
