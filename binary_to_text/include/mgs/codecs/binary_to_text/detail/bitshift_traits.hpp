#pragma once

#include <cstddef>

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
struct bitshift_traits
{
  static constexpr auto nb_decoded_bytes = EncodingTraits::nb_decoded_bytes;
  static constexpr auto nb_encoded_bytes = EncodingTraits::nb_encoded_bytes;
  static constexpr auto nb_encoded_bits = nb_encoded_bytes * 8;
  static constexpr auto nb_decoded_bits = nb_decoded_bytes * 8;
  static constexpr auto nb_index_bits = nb_decoded_bits / nb_encoded_bytes;
  static constexpr auto encoded_shift = nb_decoded_bits - nb_index_bits;
  static constexpr auto decoded_shift = nb_decoded_bits - 8;
  static constexpr auto nb_max_padding_bytes =
      nb_encoded_bytes - ((8 / nb_index_bits) + ((8 % nb_index_bits) > 0));
};
}
}
}
}
}
