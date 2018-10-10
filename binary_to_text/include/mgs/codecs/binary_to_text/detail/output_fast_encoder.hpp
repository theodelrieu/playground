#pragma once

#include <bitset>
#include <cstddef>

#include <mgs/codecs/binary_to_text/detail/bitshift_traits.hpp>
#include <mgs/codecs/binary_to_text/detail/math.hpp>
#include <mgs/codecs/binary_to_text/detail/output_slow_encoder.hpp>

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
template <typename EncodingTraits,
          std::size_t InputBytes = EncodingTraits::nb_input_bytes,
          std::size_t OutputBytes = EncodingTraits::nb_output_bytes>
struct output_fast_encoder
{
  using BitshiftTraits = bitshift_traits<InputBytes, OutputBytes>;

  template <typename OutputIterator>
  static void encode(
      std::bitset<BitshiftTraits::nb_total_input_bits> const& input_bits,
      OutputIterator out)
  {
    output_slow_encoder<EncodingTraits>::encode(input_bits, out, OutputBytes);
  }
};

template <typename EncodingTraits>
struct output_fast_encoder<EncodingTraits, 3, 4>
{
  using BitshiftTraits = bitshift_traits<3, 4>;

  template <typename OutputIterator>
  static void encode(
      std::bitset<BitshiftTraits::nb_total_input_bits> const& input_bits,
      OutputIterator out)
  {
    constexpr std::bitset<BitshiftTraits::nb_total_input_bits> mask(
        pow<2, BitshiftTraits::nb_encoded_bits>() - 1);

    out[0] = EncodingTraits::alphabet[((input_bits >> 18) & mask).to_ulong()];
    out[1] = EncodingTraits::alphabet[((input_bits >> 12) & mask).to_ulong()];
    out[2] = EncodingTraits::alphabet[((input_bits >> 6) & mask).to_ulong()];
    out[3] = EncodingTraits::alphabet[((input_bits & mask).to_ulong())];
  }
};
}
}
}
}
}
