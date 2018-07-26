#pragma once

#include <mgs/adapters/transformer_adapter.hpp>
#include <mgs/base64/detail/encoding_traits.hpp>
#include <mgs/base_n/basic_decoder.hpp>

namespace mgs
{
namespace base64
{
inline namespace v1
{
template <typename Iterator, typename Sentinel = Iterator>
using decoder = adapters::transformer_adapter<
    base_n::basic_decoder<detail::encoding_traits<>>,
    Iterator,
    Sentinel>;
}
}
}
