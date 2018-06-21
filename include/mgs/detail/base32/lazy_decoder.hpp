#pragma once

#include <ios>

#include <mgs/detail/base32/base32_alphabet.hpp>
#include <mgs/detail/input_transformers/base_n.hpp>

namespace mgs
{
namespace detail
{
struct base32_decode_traits_impl : base32_alphabet<void>
{
  static constexpr auto nb_input_bytes = 8;
  static constexpr auto nb_output_bytes = 5;
};

struct base32_decode_traits : base32_decode_traits_impl
{
  using value_type = char;
  using difference_type = std::streamoff;
  using algorithm = base_n_decode<base32_decode_traits_impl>;
};

template <typename Iterator, typename Sentinel = Iterator>
using base32_lazy_decoder =
    base_n_transformer<base32_decode_traits, Iterator, Sentinel>;
}
}
