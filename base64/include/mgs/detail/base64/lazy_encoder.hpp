#pragma once

#include <ios>

#include <mgs/detail/base64/base64_alphabet.hpp>
#include <mgs/detail/base64/base64url_alphabet.hpp>
#include <mgs/detail/base_n/base_n.hpp>

namespace mgs
{
namespace detail
{
template <typename Alphabet>
struct basic_base64_encode_traits_impl : public Alphabet
{
  static constexpr auto nb_input_bytes = 3;
  static constexpr auto nb_output_bytes = 4;
};

template <typename Alphabet>
struct basic_base64_encode_traits : basic_base64_encode_traits_impl<Alphabet>
{
  using value_type = char;
  using difference_type = std::streamoff;
  using algorithm = base_n_encode<basic_base64_encode_traits_impl<Alphabet>>;
};

using base64_encode_traits = basic_base64_encode_traits<base64_alphabet<>>;
using base64url_encode_traits =
    basic_base64_encode_traits<base64url_alphabet<>>;

template <typename Iterator, typename Sentinel = Iterator>
using base64_lazy_encoder =
    base_n_transformer<base64_encode_traits, Iterator, Sentinel>;

template <typename Iterator, typename Sentinel = Iterator>
using base64url_lazy_encoder =
    base_n_transformer<base64url_encode_traits, Iterator, Sentinel>;
}
}
