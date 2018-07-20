#pragma once

#include <algorithm>

#include <mgs/base_n/padding_policy.hpp>

namespace mgs
{
namespace base64
{
inline namespace v1
{
namespace detail
{
template <base_n::padding_policy PaddingPolicy>
struct basic_traits_url
{
  using alphabet_t = char const[64];

  static constexpr alphabet_t alphabet = {
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
      'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
      'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_'};

  static constexpr char const encoding_name[] = "base64url";
  static constexpr char const padding_character = '=';

  static constexpr auto find_char(char c)
  {
    return std::find(std::begin(alphabet), std::end(alphabet), c);
  }
};

template <base_n::padding_policy PaddingPolicy>
constexpr typename basic_traits_url<PaddingPolicy>::alphabet_t
    basic_traits_url<PaddingPolicy>::alphabet;

template <base_n::padding_policy PaddingPolicy>
constexpr char const basic_traits_url<PaddingPolicy>::encoding_name[];

template <base_n::padding_policy PaddingPolicy>
constexpr char const basic_traits_url<PaddingPolicy>::padding_character;
}
}
}
}
