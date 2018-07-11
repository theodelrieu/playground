#pragma once

#include <ios>

#include <mgs/detail/base32/base32_alphabet.hpp>
#include <mgs/detail/base_n/base_n.hpp>

namespace mgs
{
namespace detail
{
struct base32_encode_traits_impl : base32_alphabet<void>
{
  static constexpr auto nb_input_bytes = 5;
  static constexpr auto nb_output_bytes = 8;
};

struct base32_encode_traits : base32_encode_traits_impl
{
  using value_type = char;
  using difference_type = std::streamoff;
  using algorithm = base_n_encode<base32_encode_traits_impl>;
};

template <typename Iterator, typename Sentinel = Iterator>
using base32_lazy_encoder =
    base_n_transformer<base32_encode_traits, Iterator, Sentinel>;
}
}
