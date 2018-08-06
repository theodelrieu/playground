#pragma once

#include <ios>

#include <mgs/adapters/transformer_adapter.hpp>
#include <mgs/base32/detail/encoding_traits.hpp>
#include <mgs/base_n/binary_to_text/basic_encoder.hpp>

namespace mgs
{
inline namespace v1
{
namespace base32
{
template <typename Iterator, typename Sentinel = Iterator>
using encoder = adapters::transformer_adapter<
    base_n::binary_to_text::basic_encoder<detail::encoding_traits<5, 8>>,
    Iterator,
    Sentinel>;
}
}
}
