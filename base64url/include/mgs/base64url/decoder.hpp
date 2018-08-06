#pragma once

#include <mgs/adapters/transformer_adapter.hpp>
#include <mgs/base64url/detail/basic_encoding_traits.hpp>
#include <mgs/base_n/binary_to_text/basic_decoder.hpp>
#include <mgs/base_n/binary_to_text/padding_policy.hpp>

namespace mgs
{
inline namespace v1
{
namespace base64url
{
template <typename Iterator, typename Sentinel = Iterator>
using decoder = adapters::transformer_adapter<
    base_n::binary_to_text::basic_decoder<detail::basic_encoding_traits<
        4,
        3,
        base_n::binary_to_text::padding_policy::required>>,
    Iterator,
    Sentinel>;
}
}
}
