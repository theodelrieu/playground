#pragma once

#include <ios>

#include <mgs/adapters/transformer_adapter.hpp>
#include <mgs/base32hex/detail/encoding_traits.hpp>
#include <mgs/base_n/basic_encoder.hpp>

namespace mgs
{
namespace base32hex
{
inline namespace v1
{
template <typename Iterator, typename Sentinel = Iterator>
using encoder = adapters::transformer_adapter<
    base_n::basic_encoder<detail::encoding_traits<5, 8>>,
    Iterator,
    Sentinel>;
}
}
}
