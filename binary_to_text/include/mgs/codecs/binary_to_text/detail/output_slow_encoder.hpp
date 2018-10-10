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
template <typename EncodingTraits>
struct output_slow_encoder
{
  using BitshiftTraits = bitshift_traits<EncodingTraits::nb_input_bytes,
                                         EncodingTraits::nb_output_bytes>;

  template <typename OutputIterator>
  static void encode(
      std::bitset<BitshiftTraits::nb_total_input_bits> const& input_bits,
      OutputIterator out,
      std::size_t n)
  {
    constexpr std::bitset<BitshiftTraits::nb_total_input_bits> mask(
        pow<2, BitshiftTraits::nb_encoded_bits>() - 1);

    for (auto i = 0u; i < BitshiftTraits::nb_output_bytes; ++i)
    {
      auto const index =
          ((input_bits >>
            (BitshiftTraits::shift - (i * BitshiftTraits::nb_encoded_bits))) &
           mask)
              .to_ulong();
      out[i] = EncodingTraits::alphabet[index];
    }
  }
};
}
}
}
}
}
