#pragma once

#include <bitset>
#include <cstddef>

#include <mgs/codecs/binary_to_text/detail/bitshift_traits.hpp>

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
template <typename BitshiftTraits, typename RandomAccessIterator>
auto decoded_to_bitset(RandomAccessIterator it, std::size_t n)
{
  std::bitset<BitshiftTraits::nb_decoded_bits> input_bits;

  for (auto i = 0u; i < n; ++i)
  {
    decltype(input_bits) bits(it[i]);
    input_bits |= (bits << (BitshiftTraits::nb_decoded_bits - 8 - (8 * i)));
  }
  return input_bits;
}

template <typename EncodingTraits, typename RandomAccessIterator>
auto indices_to_bitset(RandomAccessIterator it, std::size_t n)
{
  using BitshiftTraits = bitshift_traits<EncodingTraits>;
  std::bitset<BitshiftTraits::nb_decoded_bits> input_bits;

  for (auto i = 0u; i < n; ++i)
  {
    decltype(input_bits) bits(EncodingTraits::index_of(it[i]));
    input_bits |= (bits << (BitshiftTraits::nb_decoded_bits -
                            BitshiftTraits::nb_index_bits -
                            (BitshiftTraits::nb_index_bits * i)));
  }
  return input_bits;
}
}
}
}
}
}
