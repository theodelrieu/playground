#pragma once

#include <mgs/base_n/binary_to_text/padding_policy.hpp>

namespace mgs
{
inline namespace v1
{
namespace base_n
{
namespace binary_to_text
{
namespace detail
{
template <typename EncodingTraits,
          padding_policy Policy = EncodingTraits::padding_policy>
struct padding_writer
{
  template <typename OutputIterator>
  static void write(OutputIterator, int)
  {
  }
};

template <typename EncodingTraits>
struct padding_writer<EncodingTraits, padding_policy::required>
{
  template <typename OutputIterator>
  static void write(OutputIterator out, int n)
  {
    while (n-- > 0)
      *out++ = EncodingTraits::padding_character;
  }
};
}
}
}
}
}
