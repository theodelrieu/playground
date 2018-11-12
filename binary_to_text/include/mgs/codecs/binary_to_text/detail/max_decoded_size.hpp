#pragma once

#include <cstddef>

#include <mgs/codecs/binary_to_text/detail/bitshift_traits.hpp>
#include <mgs/codecs/binary_to_text/detail/input_sanitizer.hpp>
#include <mgs/codecs/binary_to_text/padding_policy.hpp>

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
          padding_policy = EncodingTraits::padding_policy>
struct max_decoded_size
{
  constexpr std::size_t operator()(std::size_t encoded_size) const
  {
    using BitshiftTraits = bitshift_traits<EncodingTraits>;

    if (invalid_padding_character_pos<BitshiftTraits>(encoded_size))
      return 0;
    return (encoded_size / BitshiftTraits::nb_encoded_bytes *
            BitshiftTraits::nb_decoded_bytes) +
           (((encoded_size % BitshiftTraits::nb_encoded_bytes) *
            BitshiftTraits::nb_decoded_bytes) /
            BitshiftTraits::nb_encoded_bytes);
  }
};

template <typename EncodingTraits>
struct max_decoded_size<EncodingTraits, padding_policy::required>
{
  constexpr std::size_t operator()(std::size_t encoded_size) const
  {
    using BitshiftTraits = bitshift_traits<EncodingTraits>;

    if (encoded_size % BitshiftTraits::nb_encoded_bytes)
      return 0;
    return (encoded_size / BitshiftTraits::nb_encoded_bytes) *
           BitshiftTraits::nb_decoded_bytes;
  }
};
}
}
}
}
}
