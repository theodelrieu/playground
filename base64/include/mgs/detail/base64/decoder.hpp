#pragma once

#include <cstdint>
#include <ios>

#include <mgs/detail/base64/base64_alphabet.hpp>
#include <mgs/detail/base64/base64url_alphabet.hpp>
#include <mgs/detail/adapters/transformer_adapter.hpp>
#include <mgs/detail/base_n/decoder.hpp>
#include <mgs/detail/base_n/padding_policy.hpp>

namespace mgs
{
inline namespace v1
{
namespace detail
{
template <typename Alphabet, base_n_padding_policy PaddingPolicy>
struct basic_base64_decode_traits : public Alphabet
{
  static constexpr auto const padding_policy = PaddingPolicy;
};

using base64_decode_traits =
    basic_base64_decode_traits<base64_alphabet<>,
                               base_n_padding_policy::required>;

using base64url_decode_traits =
    basic_base64_decode_traits<base64url_alphabet<>,
                               base_n_padding_policy::required>;

using base64url_unpadded_decode_traits =
    basic_base64_decode_traits<base64url_alphabet<>,
                               base_n_padding_policy::optional>;

template <typename Iterator, typename Sentinel = Iterator>
using base64_decoder =
    transformer_adapter<base_n_decoder<base64_decode_traits>, Iterator, Sentinel>;

template <typename Iterator, typename Sentinel = Iterator>
using base64url_decoder =
    transformer_adapter<base_n_decoder<base64url_decode_traits>, Iterator, Sentinel>;

template <typename Iterator, typename Sentinel = Iterator>
using base64url_unpadded_decoder =
    transformer_adapter<base_n_decoder<base64url_unpadded_decode_traits>,
                  Iterator,
                  Sentinel>;
}
}
}
