#pragma once

#include <cstdint>
#include <ios>

#include <mgs/adapters/transformer_adapter.hpp>
#include <mgs/base_n/basic_decoder.hpp>
#include <mgs/base_n/padding_policy.hpp>
#include <mgs/detail/base16/base16_alphabet.hpp>

namespace mgs
{
inline namespace v1
{
namespace detail
{
struct base16_decode_traits : base16_alphabet<>
{
  static constexpr auto const padding_policy = base_n::padding_policy::none;
};

template <typename Iterator, typename Sentinel = Iterator>
using base16_decoder =
    adapters::transformer_adapter<base_n::basic_decoder<base16_decode_traits>,
                                  Iterator,
                                  Sentinel>;
}
}
}
