#pragma once

namespace mgs
{
namespace detail
{
template <typename>
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

  static constexpr bool is_padding_character(char)
  {
    return false;
  }
};

template <typename Dummy>
constexpr typename base16_alphabet<Dummy>::alphabet_t
    base16_alphabet<Dummy>::alphabet;
}
}
