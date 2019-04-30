#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <mgs/base32hex/detail/encoding_traits.hpp>
#include <mgs/binary_to_base/basic_codec.hpp>

namespace mgs
{
inline namespace v1
{
using base32hex =
    binary_to_base::basic_codec<detail::base32hex_encoding_traits<>>;
}
}
