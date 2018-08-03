#pragma once

#include <mgs/adapters/transformer_adapter.hpp>
#include <mgs/base64url/detail/basic_encoding_traits.hpp>
#include <mgs/base_n/basic_encoder.hpp>
#include <mgs/base_n/padding_policy.hpp>

namespace mgs
{
namespace base64url
{
inline namespace v1
{
template <typename Iterator, typename Sentinel = Iterator>
using encoder = adapters::transformer_adapter<
    base_n::basic_encoder<
        detail::basic_encoding_traits<3, 4, base_n::padding_policy::required>>,
    Iterator,
    Sentinel>;
}
}
}
