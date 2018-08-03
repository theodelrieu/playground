#pragma once

#include <ios>

#include <mgs/adapters/transformer_adapter.hpp>
#include <mgs/base32/detail/encoding_traits.hpp>
#include <mgs/base_n/basic_decoder.hpp>

namespace mgs
{
namespace base32
{
inline namespace v1
{
template <typename Iterator, typename Sentinel = Iterator>
using decoder = adapters::transformer_adapter<
    base_n::basic_decoder<detail::encoding_traits<8, 5>>,
    Iterator,
    Sentinel>;
}
}
}
