#pragma once

#include <ios>

#include <mgs/detail/base16/base16_alphabet.hpp>
#include <mgs/detail/base_n/encoder.hpp>
#include <mgs/detail/base_n/padding_policy.hpp>
#include <mgs/detail/base_n/transformer.hpp>

namespace mgs
{
namespace detail
{
struct base16_encode_algo_traits : base16_alphabet<>
{
  static constexpr auto const padding_policy = base_n_padding_policy::none;
};

struct base16_encode_traits : base16_alphabet<>
{
  using value_type = char;
  using difference_type = std::streamoff;
  using algorithm = base_n_encoder<base16_encode_algo_traits>;
};

template <typename Iterator, typename Sentinel = Iterator>
using base16_lazy_encoder =
    base_n_transformer<base16_encode_traits, Iterator, Sentinel>;
}
}
