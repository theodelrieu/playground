#pragma once

#include <cstdint>
#include <ios>

#include <mgs/detail/base64/base64_alphabet.hpp>
#include <mgs/detail/base64/base64url_alphabet.hpp>
#include <mgs/detail/base_n/decoder.hpp>
#include <mgs/detail/base_n/padding_policy.hpp>
#include <mgs/detail/base_n/transformer.hpp>

namespace mgs
{
namespace detail
{
template <typename Alphabet, base_n_padding_policy PaddingPolicy>
struct basic_base64_decode_algo_traits : public Alphabet
{
  static constexpr auto const padding_policy = PaddingPolicy;
};

template <typename Alphabet, base_n_padding_policy PaddingPolicy>
struct basic_base64_decode_traits : public Alphabet
{
  using value_type = std::uint8_t;
  using difference_type = std::streamoff;
  using algorithm =
      base_n_decoder<basic_base64_decode_algo_traits<Alphabet, PaddingPolicy>>;
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
    base_n_transformer<base64_decode_traits, Iterator, Sentinel>;

template <typename Iterator, typename Sentinel = Iterator>
using base64url_decoder =
    base_n_transformer<base64url_decode_traits, Iterator, Sentinel>;

template <typename Iterator, typename Sentinel = Iterator>
using base64url_unpadded_decoder =
    base_n_transformer<base64url_unpadded_decode_traits, Iterator, Sentinel>;
}
}
