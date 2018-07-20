#pragma once

#include <mgs/base_n/padding_policy.hpp>

namespace mgs
{
namespace base_n
{
inline namespace v1
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
