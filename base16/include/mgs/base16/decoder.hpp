#pragma once

#include <cstdint>
#include <ios>

#include <mgs/adapters/transformer_adapter.hpp>
#include <mgs/base16/detail/encoding_traits.hpp>
#include <mgs/base_n/basic_decoder.hpp>

namespace mgs
{
namespace base16
{
inline namespace v1
{
template <typename Iterator, typename Sentinel = Iterator>
using decoder = adapters::transformer_adapter<
    base_n::basic_decoder<detail::encoding_traits<2, 1>>,
    Iterator,
    Sentinel>;
}
}
}
