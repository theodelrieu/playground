#pragma once

#include <mgs/adapters/transformer_adapter.hpp>
#include <mgs/base64/detail/encoding_traits.hpp>
#include <mgs/binary_to_text/basic_decoder.hpp>

namespace mgs
{
inline namespace v1
{
namespace base64
{
template <typename Iterator, typename Sentinel = Iterator>
using decoder = adapters::transformer_adapter<
    binary_to_text::basic_decoder<detail::encoding_traits<4, 3>>,
    Iterator,
    Sentinel>;
}
}
}
