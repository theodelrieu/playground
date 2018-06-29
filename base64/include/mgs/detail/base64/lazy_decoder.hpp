#pragma once

#include <cstdint>
#include <ios>

#include <mgs/detail/base_n/base_n.hpp>
#include <mgs/detail/base64/base64_alphabet.hpp>

namespace mgs
{
namespace detail
{
struct base64_decode_traits_impl : base64_alphabet<void>
{
  static constexpr auto nb_input_bytes = 4;
  static constexpr auto nb_output_bytes = 3;
};

struct base64_decode_traits : base64_decode_traits_impl
{
  using value_type = std::uint8_t;
  using difference_type = std::streamoff;
  using algorithm = base_n_decode<base64_decode_traits_impl>;
};

template <typename Iterator, typename Sentinel = Iterator>
using base64_lazy_decoder =
    base_n_transformer<base64_decode_traits, Iterator, Sentinel>;
}
}
