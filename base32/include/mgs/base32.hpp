#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <mgs/base32/detail/encoding_traits.hpp>
#include <mgs/binary_to_text/basic_codec.hpp>

namespace mgs
{
inline namespace v1
{
using base32 = binary_to_text::basic_codec<detail::base32_encoding_traits<>>;
}
}
