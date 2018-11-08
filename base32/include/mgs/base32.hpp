#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <mgs/codecs/base32/detail/codec_traits.hpp>
#include <mgs/codecs/binary_to_text/basic_codec.hpp>

namespace mgs
{
inline namespace v1
{
using base32 =
    codecs::binary_to_text::basic_codec<codecs::base32::detail::codec_traits>;
}
}
