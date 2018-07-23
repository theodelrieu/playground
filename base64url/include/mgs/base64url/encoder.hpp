#pragma once

#include <mgs/adapters/transformer_adapter.hpp>
#include <mgs/base64url/detail/basic_traits.hpp>
#include <mgs/base_n/basic_encoder.hpp>
#include <mgs/base_n/padding_policy.hpp>

namespace mgs
{
namespace base64url
{
inline namespace v1
{
template <typename Iterator, typename Sentinel = Iterator>
using encoder =
    adapters::transformer_adapter<base_n::basic_encoder<detail::basic_traits<
                                      base_n::padding_policy::required>>,
                                  Iterator,
                                  Sentinel>;

template <typename Iterator, typename Sentinel = Iterator>
using nopad_encoder = adapters::transformer_adapter<
    base_n::basic_encoder<detail::basic_traits<base_n::padding_policy::none>>,
    Iterator,
    Sentinel>;
}
}
}
