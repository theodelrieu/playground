#pragma once

#include <cstdint>
#include <ios>

#include <mgs/detail/base16/base16_alphabet.hpp>
#include <mgs/detail/base_n/decoder.hpp>
#include <mgs/detail/base_n/padding_policy.hpp>
#include <mgs/detail/base_n/transformer.hpp>

namespace mgs
{
inline namespace v1
{
namespace detail
{
struct base16_decode_algo_traits : base16_alphabet<>
{
  static constexpr auto const padding_policy = base_n_padding_policy::none;
};

struct base16_decode_traits : base16_alphabet<>
{
  using value_type = std::uint8_t;
  using difference_type = std::streamoff;
  using algorithm = base_n_decoder<base16_decode_algo_traits>;
};

template <typename Iterator, typename Sentinel = Iterator>
using base16_decoder =
    base_n_transformer<base16_decode_traits, Iterator, Sentinel>;
}
}
}
