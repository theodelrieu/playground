#pragma once

#include <mgs/codecs/binary_to_text/padding_policy.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

namespace mgs
{
namespace detail
{
template <typename EncodingTraits,
          codecs::binary_to_text::padding_policy =
              EncodingTraits::padding_policy>
struct encoded_input_reader
{
  template <typename Iterator, typename Sentinel>
  static char read(Iterator& current, Sentinel const sent)
  {
    if (current == sent)
    {
      throw exceptions::unexpected_eof_error{"unexpected end of input"};
    }
    return *current++;
  }
};

template <typename EncodingTraits>
struct encoded_input_reader<EncodingTraits,
                            codecs::binary_to_text::padding_policy::optional>
{
  template <typename Iterator, typename Sentinel>
  static char read(Iterator& current, Sentinel const sent)
  {
    if (current == sent)
      return EncodingTraits::padding_character;
    return *current++;
  }
};
}
}
