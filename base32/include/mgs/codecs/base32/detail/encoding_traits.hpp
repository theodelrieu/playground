#pragma once

#include <algorithm>
#include <iterator>
#include <cstddef>

#include <mgs/codecs/binary_to_text/padding_policy.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace base32
{
namespace detail
{
struct encoding_traits
{
  using alphabet_t = char const[32];

  static constexpr alphabet_t alphabet = {
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
      'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
      'W', 'X', 'Y', 'Z', '2', '3', '4', '5', '6', '7'};

  static constexpr char const padding_character = '=';
  static constexpr auto const padding_policy =
      binary_to_text::padding_policy::required;
  static constexpr auto const nb_decoded_bytes = 5;
  static constexpr auto const nb_encoded_bytes = 8;

  static int find_char(char c)
  {
    // FIXME lookup table
    auto it = std::find(std::begin(alphabet), std::end(alphabet), c);
    if (it == std::end(alphabet))
      return -1;
    return std::distance(std::begin(alphabet), it);
  }
};

constexpr encoding_traits::alphabet_t encoding_traits::alphabet;
constexpr char const encoding_traits::padding_character;
}
}
}
}
}
