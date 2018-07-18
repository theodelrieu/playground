#pragma once

#include <algorithm>

namespace mgs
{
inline namespace v1
{
namespace detail
{
template <typename = void>
struct base64_alphabet
{
  using alphabet_t = char const[64];

  static constexpr alphabet_t alphabet = {
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
      'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
      'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

  static constexpr char const encoding_name[] = "base64";
  static constexpr char const padding_character = '=';

  static constexpr auto find_char(char c)
  {
    return std::find(std::begin(alphabet), std::end(alphabet), c);
  }
};

template <typename Dummy>
constexpr typename base64_alphabet<Dummy>::alphabet_t
    base64_alphabet<Dummy>::alphabet;

template <typename Dummy>
constexpr char const base64_alphabet<Dummy>::encoding_name[];

template <typename Dummy>
constexpr char const base64_alphabet<Dummy>::padding_character;
}
}
}
