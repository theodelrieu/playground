#pragma once

namespace mgs
{
namespace detail
{
template <base_n_padding_policy PaddingPolicy>
struct base64url_alphabet
{
  using alphabet_t = char const[64];

  static constexpr alphabet_t alphabet = {
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
      'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
      'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_'};

  static constexpr char const encoding_name[] = "base64url";
  static constexpr auto const padding_policy = PaddingPolicy;

  static constexpr bool is_padding_character(char c)
  {
    return c == '=';
  }
};

template <base_n_padding_policy P>
constexpr
    typename base64url_alphabet<P>::alphabet_t base64url_alphabet<P>::alphabet;

template <base_n_padding_policy P>
constexpr char const base64url_alphabet<P>::encoding_name[];
}
}
