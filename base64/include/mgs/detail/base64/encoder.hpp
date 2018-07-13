#pragma once

#include <ios>

#include <mgs/detail/base64/base64_alphabet.hpp>
#include <mgs/detail/base64/base64url_alphabet.hpp>
#include <mgs/detail/base_n/encoder.hpp>
#include <mgs/detail/base_n/transformer.hpp>

namespace mgs
{
namespace detail
{
template <typename Alphabet, base_n_padding_policy PaddingPolicy>
struct basic_base64_encode_algo_traits : public Alphabet
{
  static constexpr auto const padding_policy = PaddingPolicy;
};

template <typename Alphabet, base_n_padding_policy PaddingPolicy>
struct basic_base64_encode_traits : public Alphabet
{
  using value_type = char;
  using difference_type = std::streamoff;
  using algorithm =
      base_n_encoder<basic_base64_encode_algo_traits<Alphabet, PaddingPolicy>>;
};

using base64_encode_traits =
    basic_base64_encode_traits<base64_alphabet<>,
                               base_n_padding_policy::required>;
using base64url_encode_traits =
    basic_base64_encode_traits<base64url_alphabet<>,
                               base_n_padding_policy::required>;

using base64url_unpadded_encode_traits =
    basic_base64_encode_traits<base64url_alphabet<>,
                               base_n_padding_policy::none>;

template <typename Iterator, typename Sentinel = Iterator>
using base64_encoder =
    base_n_transformer<base64_encode_traits, Iterator, Sentinel>;

template <typename Iterator, typename Sentinel = Iterator>
using base64url_encoder =
    base_n_transformer<base64url_encode_traits, Iterator, Sentinel>;

template <typename Iterator, typename Sentinel = Iterator>
using base64url_unpadded_encoder =
    base_n_transformer<base64url_unpadded_encode_traits, Iterator, Sentinel>;
}
}
