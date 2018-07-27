#pragma once

#include <algorithm>

#include <mgs/base_n/padding_policy.hpp>

namespace mgs
{
namespace base32hex
{
inline namespace v1
{
namespace detail
{
template <typename = void>
struct encoding_traits
{
  using alphabet_t = char const[32];

  static constexpr alphabet_t alphabet = {
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A',
      'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
      'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V'};

  static constexpr char const encoding_name[] = "base32hex";
  static constexpr char const padding_character = '=';
  static constexpr auto const padding_policy = base_n::padding_policy::required;

  static constexpr auto find_char(char c)
  {
    return std::find(std::begin(alphabet), std::end(alphabet), c);
  }
};

template <typename Dummy>
constexpr typename encoding_traits<Dummy>::alphabet_t
    encoding_traits<Dummy>::alphabet;

template <typename Dummy>
constexpr char const encoding_traits<Dummy>::encoding_name[];

template <typename Dummy>
constexpr char const encoding_traits<Dummy>::padding_character;
}
}
}
}
