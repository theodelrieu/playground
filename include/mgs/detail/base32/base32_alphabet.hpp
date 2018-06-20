#pragma once

namespace mgs
{
namespace detail
{
template <typename>
struct base32_alphabet
{
  using alphabet_t = char const[32];

  static constexpr alphabet_t alphabet = {
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
      'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
      'W', 'X', 'Y', 'Z', '2', '3', '4', '5', '6', '7'};
};

template <typename Dummy>
constexpr typename base32_alphabet<Dummy>::alphabet_t
    base32_alphabet<Dummy>::alphabet;
}
}
