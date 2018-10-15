#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>

#include <mgs/codecs/binary_to_text/padding_policy.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace base16
{
namespace detail
{
struct encoding_traits
{
  using alphabet_t = char const[16];

  static constexpr alphabet_t alphabet = {'0',
                                          '1',
                                          '2',
                                          '3',
                                          '4',
                                          '5',
                                          '6',
                                          '7',
                                          '8',
                                          '9',
                                          'A',
                                          'B',
                                          'C',
                                          'D',
                                          'E',
                                          'F'};

  static constexpr auto const nb_encoded_bytes = 2;
  static constexpr auto const nb_decoded_bytes = 1;

  static int find_char(char c)
  {
    // toupper is meh...
    if (c >= 'a' && c <= 'z')
      c -= 32;
    auto it = std::find(std::begin(alphabet), std::end(alphabet), c);
    if (it == std::end(alphabet))
      return -1;
    return std::distance(std::begin(alphabet), it);
  }

  static constexpr auto const padding_policy =
      binary_to_text::padding_policy::none;
};

constexpr encoding_traits::alphabet_t encoding_traits::alphabet;
}
}
}
}
}
