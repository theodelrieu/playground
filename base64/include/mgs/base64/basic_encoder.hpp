#pragma once

#include <mgs/adapters/transformer_adapter.hpp>
#include <mgs/base64/detail/basic_traits.hpp>
#include <mgs/base_n/basic_encoder.hpp>

namespace mgs
{
namespace base64
{
inline namespace v1
{
using encode_traits = detail::basic_traits<base_n::padding_policy::required>;

template <typename Iterator, typename Sentinel = Iterator>
using basic_encoder =
    adapters::transformer_adapter<base_n::basic_encoder<encode_traits>,
                                  Iterator,
                                  Sentinel>;
}
}
}
