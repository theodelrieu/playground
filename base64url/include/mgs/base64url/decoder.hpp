#pragma once

#include <mgs/adapters/transformer_adapter.hpp>
#include <mgs/base64url/detail/basic_traits.hpp>
#include <mgs/base_n/basic_decoder.hpp>
#include <mgs/base_n/padding_policy.hpp>

namespace mgs
{
namespace base64url
{
inline namespace v1
{
template <typename Iterator, typename Sentinel = Iterator>
using decoder =
    adapters::transformer_adapter<base_n::basic_decoder<detail::basic_traits<
                                      base_n::padding_policy::required>>,
                                  Iterator,
                                  Sentinel>;

template <typename Iterator, typename Sentinel = Iterator>
using nopad_decoder =
    adapters::transformer_adapter<base_n::basic_decoder<detail::basic_traits<
                                      base_n::padding_policy::optional>>,
                                  Iterator,
                                  Sentinel>;
}
}
}
