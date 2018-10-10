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
template <std::size_t InputBytes, std::size_t OutputBytes>
struct bitshift_traits
{
  static constexpr auto nb_input_bytes = InputBytes;
  static constexpr auto nb_output_bytes = OutputBytes;
  static constexpr auto nb_total_input_bits =
      detail::round_to_multiple_of<nb_input_bytes * 8, nb_output_bytes>();
  static constexpr auto nb_extra_input_bits = nb_total_input_bits % 8;
  static constexpr auto nb_encoded_bits = nb_total_input_bits / nb_output_bytes;
  static constexpr auto shift = nb_total_input_bits - nb_encoded_bits;
};
}
}
}
}
}
