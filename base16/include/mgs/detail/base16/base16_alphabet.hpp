#pragma once

#include <algorithm>

namespace mgs
{
inline namespace v1
{
namespace detail
{
template <typename = void>
struct base16_alphabet
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

  static constexpr auto find_char(char c)
  {
    // toupper is meh...
    if (c >= 'a' && c <= 'z')
      c -= 32;
    return std::find(std::begin(alphabet), std::end(alphabet), c);
  }

  static constexpr char const encoding_name[] = "base16";
};

template <typename Dummy>
constexpr typename base16_alphabet<Dummy>::alphabet_t
    base16_alphabet<Dummy>::alphabet;

template <typename Dummy>
constexpr char const base16_alphabet<Dummy>::encoding_name[];
}
}
}
