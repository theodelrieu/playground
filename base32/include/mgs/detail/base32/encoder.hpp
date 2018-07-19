#pragma once

#include <ios>

#include <mgs/detail/adapters/transformer_adapter.hpp>
#include <mgs/detail/base32/base32_alphabet.hpp>
#include <mgs/detail/base32/base32hex_alphabet.hpp>
#include <mgs/detail/base_n/encoder.hpp>
#include <mgs/detail/base_n/padding_policy.hpp>

namespace mgs
{
inline namespace v1
{
namespace detail
{

template <typename Alphabet, base_n_padding_policy PaddingPolicy>
struct basic_base32_encode_traits : public Alphabet
{
  static constexpr auto const padding_policy = PaddingPolicy;
};

using base32_encode_traits =
    basic_base32_encode_traits<base32_alphabet<>,
                               base_n_padding_policy::required>;

using base32hex_encode_traits =
    basic_base32_encode_traits<base32hex_alphabet<>,
                               base_n_padding_policy::required>;

template <typename Iterator, typename Sentinel = Iterator>
using base32_encoder = transformer_adapter<base_n_encoder<base32_encode_traits>,
                                           Iterator,
                                           Sentinel>;

template <typename Iterator, typename Sentinel = Iterator>
using base32hex_encoder =
    transformer_adapter<base_n_encoder<base32hex_encode_traits>,
                        Iterator,
                        Sentinel>;
}
}
}
