#pragma once

#include <algorithm>
#include <cstddef>

#include <mgs/base_n/padding_policy.hpp>

namespace mgs
{
namespace base16
{
inline namespace v1
{
namespace detail
{
template <std::size_t In, std::size_t Out>
struct encoding_traits
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

  static constexpr auto const nb_input_bytes = In;
  static constexpr auto const nb_output_bytes = Out;

  static constexpr auto find_char(char c)
  {
    // toupper is meh...
    if (c >= 'a' && c <= 'z')
      c -= 32;
    return std::find(std::begin(alphabet), std::end(alphabet), c);
  }

  static constexpr char const encoding_name[] = "base16";
  static constexpr auto const padding_policy = base_n::padding_policy::none;
};

template <std::size_t In, std::size_t Out>
constexpr typename encoding_traits<In, Out>::alphabet_t
    encoding_traits<In, Out>::alphabet;

template <std::size_t In, std::size_t Out>
constexpr char const encoding_traits<In, Out>::encoding_name[];
}
}
}
}
