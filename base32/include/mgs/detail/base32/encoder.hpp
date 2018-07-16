#pragma once

#include <ios>

#include <mgs/detail/base32/base32_alphabet.hpp>
#include <mgs/detail/base32/base32hex_alphabet.hpp>
#include <mgs/detail/base_n/encoder.hpp>
#include <mgs/detail/base_n/padding_policy.hpp>
#include <mgs/detail/base_n/transformer.hpp>

namespace mgs
{
inline namespace v1
{
namespace detail
{

template <typename Alphabet, base_n_padding_policy PaddingPolicy>
struct basic_base32_encode_algo_traits : public Alphabet
{
  static constexpr auto const padding_policy = PaddingPolicy;
};

template <typename Alphabet, base_n_padding_policy PaddingPolicy>
struct basic_base32_encode_traits : public Alphabet
{
  using value_type = char;
  using difference_type = std::streamoff;
  using algorithm =
      base_n_encoder<basic_base32_encode_algo_traits<Alphabet, PaddingPolicy>>;
};

using base32_encode_traits =
    basic_base32_encode_traits<base32_alphabet<>,
                               base_n_padding_policy::required>;

using base32hex_encode_traits =
    basic_base32_encode_traits<base32hex_alphabet<>,
                               base_n_padding_policy::required>;

template <typename Iterator, typename Sentinel = Iterator>
using base32_encoder =
    base_n_transformer<base32_encode_traits, Iterator, Sentinel>;

template <typename Iterator, typename Sentinel = Iterator>
using base32hex_encoder =
    base_n_transformer<base32hex_encode_traits, Iterator, Sentinel>;
}
}
}
