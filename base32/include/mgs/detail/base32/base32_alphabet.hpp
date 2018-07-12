#pragma once

#include <mgs/detail/base_n/padding_policy.hpp>

namespace mgs
{
namespace detail
{
template <base_n_padding_policy PaddingPolicy>
struct base32_alphabet
{
  using alphabet_t = char const[32];

  static constexpr alphabet_t alphabet = {
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
      'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
      'W', 'X', 'Y', 'Z', '2', '3', '4', '5', '6', '7'};

  static constexpr char const encoding_name[] = "base32";
  static constexpr auto const padding_policy = PaddingPolicy;

  static constexpr bool is_padding_character(char c)
  {
    return c == '=';
  }
};

template <base_n_padding_policy P>
constexpr typename base32_alphabet<P>::alphabet_t base32_alphabet<P>::alphabet;

template <base_n_padding_policy P>
constexpr char const base32_alphabet<P>::encoding_name[];
}
}
