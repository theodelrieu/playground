#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <mgs/base16/detail/encoding_traits.hpp>
#include <mgs/binary_to_base/basic_codec.hpp>

namespace mgs
{
inline namespace v1
{
using base16 = binary_to_base::basic_codec<detail::base16_encoding_traits<>>;
}
}
