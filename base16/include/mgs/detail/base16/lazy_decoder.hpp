#pragma once

#include <cstdint>
#include <ios>

#include <mgs/detail/base16/base16_alphabet.hpp>
#include <mgs/detail/base_n/base_n.hpp>
#include <mgs/detail/base_n/padding_policy.hpp>

namespace mgs
{
namespace detail
{
struct base16_decode_common_traits : base16_alphabet<>
{
  static constexpr auto const nb_input_bytes = 2;
  static constexpr auto const nb_output_bytes = 1;
};

struct base16_decode_algo_traits : base16_decode_common_traits
{
  static constexpr auto const padding_policy = base_n_padding_policy::none;
};

struct base16_decode_traits : base16_decode_common_traits
{
  using value_type = std::uint8_t;
  using difference_type = std::streamoff;
  using algorithm = base_n_decode<base16_decode_algo_traits>;
};

template <typename Iterator, typename Sentinel = Iterator>
using base16_lazy_decoder =
    base_n_transformer<base16_decode_traits, Iterator, Sentinel>;
}
}
