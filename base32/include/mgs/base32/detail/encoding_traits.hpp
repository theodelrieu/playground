#pragma once

#include <algorithm>
#include <cstddef>

#include <mgs/base_n/binary_to_text/padding_policy.hpp>

namespace mgs
{
inline namespace v1
{
namespace base32
{
namespace detail
{
template <std::size_t In, std::size_t Out>
struct encoding_traits
{
  using alphabet_t = char const[32];

  static constexpr alphabet_t alphabet = {
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
      'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
      'W', 'X', 'Y', 'Z', '2', '3', '4', '5', '6', '7'};

  static constexpr char const encoding_name[] = "base32";
  static constexpr char const padding_character = '=';
  static constexpr auto const padding_policy =
      base_n::binary_to_text::padding_policy::required;
  static constexpr auto const nb_input_bytes = In;
  static constexpr auto const nb_output_bytes = Out;

  static constexpr auto find_char(char c)
  {
    return std::find(std::begin(alphabet), std::end(alphabet), c);
  }
};

template <std::size_t In, std::size_t Out>
constexpr typename encoding_traits<In, Out>::alphabet_t
    encoding_traits<In, Out>::alphabet;

template <std::size_t In, std::size_t Out>
constexpr char const encoding_traits<In, Out>::encoding_name[];

template <std::size_t In, std::size_t Out>
constexpr char const encoding_traits<In, Out>::padding_character;
}
}
}
}