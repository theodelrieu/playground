#pragma once

#include <mgs/adapters/transformer_adapter.hpp>
#include <mgs/codecs/base64url/detail/basic_encoding_traits.hpp>
#include <mgs/codecs/binary_to_text/basic_encoder.hpp>
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
using nopad_encoder = adapters::transformer_adapter<
    binary_to_text::basic_encoder<
        detail::
            basic_encoding_traits<3, 4, binary_to_text::padding_policy::none>>,
    Iterator,
    Sentinel>;
}
}
}
}
