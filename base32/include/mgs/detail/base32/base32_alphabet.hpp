#pragma once

namespace mgs
{
namespace detail
{
template <typename = void>
struct base32_alphabet
{
  using alphabet_t = char const[32];

  static constexpr alphabet_t alphabet = {
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
      'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
      'W', 'X', 'Y', 'Z', '2', '3', '4', '5', '6', '7'};

  static constexpr char const encoding_name[] = "base32";
  static constexpr char const padding_character = '=';
};

template <typename Dummy>
constexpr typename base32_alphabet<Dummy>::alphabet_t
    base32_alphabet<Dummy>::alphabet;

template <typename Dummy>
constexpr char const base32_alphabet<Dummy>::encoding_name[];
}
}
