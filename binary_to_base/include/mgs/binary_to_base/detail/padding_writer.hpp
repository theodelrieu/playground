#pragma once

#include <mgs/binary_to_base/padding_policy.hpp>
#include <mgs/meta/ssize_t.hpp>

namespace mgs
{
namespace detail
{
template <typename EncodingTraits,
          binary_to_base::padding_policy Policy =
              EncodingTraits::padding_policy>
struct padding_writer
{
  template <typename OutputIterator>
  static void write(OutputIterator, meta::ssize_t)
  {
  }
};

template <typename EncodingTraits>
struct padding_writer<EncodingTraits, binary_to_base::padding_policy::required>
{
  template <typename OutputIterator>
  static void write(OutputIterator out, meta::ssize_t n)
  {
    while (n-- > 0)
      *out++ = EncodingTraits::padding_character;
  }
};
}
}
