#pragma once

#include <algorithm>
#include <cstddef>

#include <mgs/codecs/binary_to_text/padding_policy.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace base64url
{
namespace detail
{
template <std::size_t In,
          std::size_t Out,
          binary_to_text::padding_policy PaddingPolicy>
struct basic_encoding_traits
{
  using alphabet_t = char const[64];

  static constexpr alphabet_t alphabet = {
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
      'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
      'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_'};

  static constexpr auto const padding_character = '=';
  static constexpr char const encoding_name[] = "base64url";
  static constexpr auto const padding_policy = PaddingPolicy;
  static constexpr auto const nb_input_bytes = In;
  static constexpr auto const nb_output_bytes = Out;

  static constexpr auto find_char(char c)
  {
    return std::find(std::begin(alphabet), std::end(alphabet), c);
  }
};

template <std::size_t In,
          std::size_t Out,
          binary_to_text::padding_policy PaddingPolicy>
constexpr typename basic_encoding_traits<In, Out, PaddingPolicy>::alphabet_t
    basic_encoding_traits<In, Out, PaddingPolicy>::alphabet;

template <std::size_t In,
          std::size_t Out,
          binary_to_text::padding_policy PaddingPolicy>
constexpr char const
    basic_encoding_traits<In, Out, PaddingPolicy>::encoding_name[];

template <std::size_t In,
          std::size_t Out,
          binary_to_text::padding_policy PaddingPolicy>
constexpr char const
    basic_encoding_traits<In, Out, PaddingPolicy>::padding_character;
}
}
}
}
}
