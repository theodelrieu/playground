#pragma once

#include <algorithm>
#include <iterator>

#include <mgs/codecs/binary_to_text/padding_policy.hpp>

namespace mgs
{
namespace codecs
{
namespace base32hex
{
namespace detail
{
template <typename Dummy = void>
struct encoding_traits
{
  using lookup_table_t = int const[256];

  static constexpr lookup_table_t lookup_table = {
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0,  1,  2,  3,  4,  5,
      6,  7,  8,  9,  -1, -1, -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15, 16,
      17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1};

  using alphabet_t = char const[32];

  static constexpr alphabet_t alphabet = {
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A',
      'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
      'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V'};

  static constexpr char const padding_character = '=';
  static constexpr auto const padding_policy =
      binary_to_text::padding_policy::required;

  static int index_of(char c)
  {
    return lookup_table[static_cast<unsigned char>(c)];
  }
};

template <typename T>
constexpr typename encoding_traits<T>::alphabet_t encoding_traits<T>::alphabet;

template <typename T>
constexpr typename encoding_traits<T>::lookup_table_t
    encoding_traits<T>::lookup_table;

template <typename T>
constexpr char const encoding_traits<T>::padding_character;
}
}
}
}
