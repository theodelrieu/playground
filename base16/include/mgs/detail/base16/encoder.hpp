#pragma once

#include <ios>

#include <mgs/detail/adapters/transformer_adapter.hpp>
#include <mgs/detail/base16/base16_alphabet.hpp>
#include <mgs/detail/base_n/encoder.hpp>
#include <mgs/detail/base_n/padding_policy.hpp>

namespace mgs
{
inline namespace v1
{
namespace detail
{
struct base16_encode_traits : base16_alphabet<>
{
  static constexpr auto const padding_policy = base_n_padding_policy::none;
};

template <typename Iterator, typename Sentinel = Iterator>
using base16_encoder = transformer_adapter<base_n_encoder<base16_encode_traits>,
                                           Iterator,
                                           Sentinel>;
}
}
}
