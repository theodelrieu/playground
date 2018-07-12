#pragma once

#include <ios>

#include <mgs/detail/base32/base32_alphabet.hpp>
#include <mgs/detail/base_n/base_n.hpp>
#include <mgs/detail/base_n/padding_policy.hpp>

namespace mgs
{
namespace detail
{
template <typename Alphabet>
struct basic_base32_encode_common_traits : public Alphabet
{
  static constexpr auto nb_input_bytes = 5;
  static constexpr auto nb_output_bytes = 8;
};

template <typename Alphabet, base_n_padding_policy PaddingPolicy>
struct basic_base32_encode_algo_traits
  : basic_base32_encode_common_traits<Alphabet>
{
  static constexpr auto const padding_policy = PaddingPolicy;
};

template <typename Alphabet, base_n_padding_policy PaddingPolicy>
struct basic_base32_encode_traits : basic_base32_encode_common_traits<Alphabet>
{
  using value_type = char;
  using difference_type = std::streamoff;
  using algorithm =
      base_n_encode<basic_base32_encode_algo_traits<Alphabet, PaddingPolicy>>;
};

using base32_encode_traits =
    basic_base32_encode_traits<base32_alphabet<>,
                               base_n_padding_policy::required>;

template <typename Iterator, typename Sentinel = Iterator>
using base32_lazy_encoder =
    base_n_transformer<base32_encode_traits, Iterator, Sentinel>;
}
}
