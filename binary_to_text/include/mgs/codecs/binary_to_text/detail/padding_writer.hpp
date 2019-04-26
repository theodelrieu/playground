#pragma once

#include <mgs/meta/ssize_t.hpp>
#include <mgs/codecs/binary_to_text/padding_policy.hpp>

namespace mgs
{
namespace detail
{
template <typename EncodingTraits,
          codecs::binary_to_text::padding_policy Policy =
              EncodingTraits::padding_policy>
struct padding_writer
{
  template <typename OutputIterator>
  static void write(OutputIterator, meta::ssize_t)
  {
  }
};

template <typename EncodingTraits>
struct padding_writer<EncodingTraits,
                      codecs::binary_to_text::padding_policy::required>
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
