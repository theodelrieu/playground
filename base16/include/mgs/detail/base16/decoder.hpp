#pragma once

#include <cstdint>
#include <ios>

#include <mgs/detail/adapters/transformer_adapter.hpp>
#include <mgs/detail/base16/base16_alphabet.hpp>
#include <mgs/detail/base_n/decoder.hpp>
#include <mgs/detail/base_n/padding_policy.hpp>

namespace mgs
{
inline namespace v1
{
namespace detail
{
struct base16_decode_traits : base16_alphabet<>
{
  static constexpr auto const padding_policy = base_n_padding_policy::none;
};

template <typename Iterator, typename Sentinel = Iterator>
using base16_decoder = transformer_adapter<base_n_decoder<base16_decode_traits>,
                                           Iterator,
                                           Sentinel>;
}
}
}
