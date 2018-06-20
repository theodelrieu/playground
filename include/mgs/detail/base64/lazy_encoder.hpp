#pragma once

#include <ios>

#include <mgs/detail/base64/base64_alphabet.hpp>
#include <mgs/detail/input_transformers/base_n.hpp>

namespace mgs
{
namespace detail
{
struct base64_encode_traits_impl : base64_alphabet<void>
{
  static constexpr auto nb_input_bytes = 3;
  static constexpr auto nb_output_bytes = 4;
};

struct base64_encode_traits : base64_encode_traits_impl
{
  using value_type = char;
  using difference_type = std::streamoff;
  using algorithm = base_n_encode<base64_encode_traits_impl>;
};

template <typename Iterator, typename Sentinel = Iterator>
using base64_lazy_encoder =
    base_n_transformer<base64_encode_traits, Iterator, Sentinel>;
}
}
