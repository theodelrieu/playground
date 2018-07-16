#pragma once

#include <algorithm>

namespace mgs
{
namespace detail
{
template <typename = void>
struct base32hex_alphabet
{
  using alphabet_t = char const[32];

  static constexpr alphabet_t alphabet = {
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A',
      'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
      'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V'};

  static constexpr char const encoding_name[] = "base32hex";
  static constexpr char const padding_character = '=';

  static constexpr auto find_char(char c)
  {
    return std::find(std::begin(alphabet), std::end(alphabet), c);
  }
};

template <typename Dummy>
constexpr typename base32hex_alphabet<Dummy>::alphabet_t
    base32hex_alphabet<Dummy>::alphabet;

template <typename Dummy>
constexpr char const base32hex_alphabet<Dummy>::encoding_name[];
}
}
