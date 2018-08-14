#pragma once

#include <mgs/adapters/transformer_adapter.hpp>
#include <mgs/codecs/base64url/detail/basic_encoding_traits.hpp>
#include <mgs/codecs/binary_to_text/basic_decoder.hpp>
#include <mgs/codecs/binary_to_text/padding_policy.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace base64url
{
template <typename Iterator, typename Sentinel = Iterator>
using decoder = adapters::transformer_adapter<binary_to_text::basic_decoder<
    Iterator,
    Sentinel,
    detail::
        basic_encoding_traits<4, 3, binary_to_text::padding_policy::required>>>;
}
}
}
}
