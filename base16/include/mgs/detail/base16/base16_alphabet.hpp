#pragma once

#include <mgs/detail/base_n/padding_policy.hpp>

namespace mgs
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

  static constexpr char const encoding_name[] = "base16";
};

template <typename Dummy>
constexpr typename base16_alphabet<Dummy>::alphabet_t
    base16_alphabet<Dummy>::alphabet;

template <typename Dummy>
constexpr char const base16_alphabet<Dummy>::encoding_name[];
}
}
