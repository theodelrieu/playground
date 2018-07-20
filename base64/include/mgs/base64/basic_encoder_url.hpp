#pragma once

#include <mgs/adapters/transformer_adapter.hpp>
#include <mgs/base64/detail/basic_traits_url.hpp>
#include <mgs/base_n/basic_encoder.hpp>

namespace mgs
{
namespace base64
{
inline namespace v1
{
using url_encode_traits =
    detail::basic_traits_url<base_n::padding_policy::required>;

template <typename Iterator, typename Sentinel = Iterator>
using basic_encoder_url =
    adapters::transformer_adapter<base_n::basic_encoder<url_encode_traits>,
                                  Iterator,
                                  Sentinel>;
}
}
}

