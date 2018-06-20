#pragma once

#include <ios>

#include <mgs/detail/input_transformers/base_n.hpp>

namespace mgs
{
namespace detail
{
template <typename>
struct base64_encode_traits_impl
{
  static constexpr auto nb_input_bytes = 3;
  static constexpr auto nb_output_bytes = 4;

  using alphabet_t = char const[64];

  static constexpr alphabet_t alphabet = {
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
      'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
      'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};
};

template <typename Dummy>
constexpr typename base64_encode_traits_impl<Dummy>::alphabet_t
    base64_encode_traits_impl<Dummy>::alphabet;

struct base64_encode_traits : base64_encode_traits_impl<void>
{
  using value_type = char;
  using difference_type = std::streamoff;
  using algorithm = base_n_encode<base64_encode_traits_impl<void>>;
};

template <typename Iterator, typename Sentinel = Iterator>
using base64_lazy_encoder =
    base_n_transformer<base64_encode_traits, Iterator, Sentinel>;
}
}
