#pragma once

#include <ios>

#include <mgs/detail/base16/base16_alphabet.hpp>
#include <mgs/detail/input_transformers/base_n.hpp>

namespace mgs
{
namespace detail
{
struct base16_encode_traits_impl : base16_alphabet<void>
{
  static constexpr auto nb_input_bytes = 1;
  static constexpr auto nb_output_bytes = 2;
};

struct base16_encode_traits : base16_encode_traits_impl
{
  using value_type = char;
  using difference_type = std::streamoff;
  using algorithm = base_n_encode<base16_encode_traits_impl>;
};

template <typename Iterator, typename Sentinel = Iterator>
using base16_lazy_encoder =
    base_n_transformer<base16_encode_traits, Iterator, Sentinel>;
}
}
