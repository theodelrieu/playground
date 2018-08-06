#pragma once

#include <mgs/adapters/transformer_adapter.hpp>
#include <mgs/base64/detail/encoding_traits.hpp>
#include <mgs/base_n/binary_to_text/basic_encoder.hpp>

namespace mgs
{
inline namespace v1
{
namespace base64
{
template <typename Iterator, typename Sentinel = Iterator>
using encoder = adapters::transformer_adapter<
    base_n::binary_to_text::basic_encoder<detail::encoding_traits<3, 4>>,
    Iterator,
    Sentinel>;
}
}
}
