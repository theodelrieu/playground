#pragma once

#include <mgs/base_n/padding_policy.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

namespace mgs
{
inline namespace v1
{
namespace base_n
{
namespace detail
{
template <typename EncodingTraits,
          padding_policy = EncodingTraits::padding_policy>
struct encoded_input_reader
{
  template <typename Iterator, typename Sentinel>
  static char read(Iterator& current, Sentinel const sent)
  {
    if (current == sent)
    {
      throw exceptions::unexpected_eof_error{EncodingTraits::encoding_name,
                                             "unexpected end of input"};
    }
    return *current++;
  }
};

template <typename EncodingTraits>
struct encoded_input_reader<EncodingTraits, padding_policy::optional>
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
}
}
