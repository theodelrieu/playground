#pragma once

#include <cstdint>
#include <ios>

#include <mgs/detail/base_n/base_n.hpp>
#include <mgs/detail/base64/base64_alphabet.hpp>
#include <mgs/detail/base64/base64url_alphabet.hpp>

namespace mgs
{
namespace detail
{
template <typename Alphabet>
struct basic_base64_decode_traits_impl : public Alphabet
{
  static constexpr auto nb_input_bytes = 4;
  static constexpr auto nb_output_bytes = 3;
};

template <typename Alphabet>
struct basic_base64_decode_traits : basic_base64_decode_traits_impl<Alphabet>
{
  using value_type = std::uint8_t;
  using difference_type = std::streamoff;
  using algorithm = base_n_decode<basic_base64_decode_traits_impl<Alphabet>>;
};

using base64_decode_traits = basic_base64_decode_traits<
    base64_alphabet<base_n_padding_policy::required>>;
using base64url_decode_traits = basic_base64_decode_traits<
    base64url_alphabet<base_n_padding_policy::required>>;

template <typename Iterator, typename Sentinel = Iterator>
using base64_lazy_decoder =
    base_n_transformer<base64_decode_traits, Iterator, Sentinel>;

template <typename Iterator, typename Sentinel = Iterator>
using base64url_lazy_decoder =
    base_n_transformer<base64url_decode_traits, Iterator, Sentinel>;
}
}
