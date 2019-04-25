#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <mgs/codecs/binary_to_text/basic_codec.hpp>
#include <mgs/codecs/base64/detail/encoding_traits.hpp>

namespace mgs
{
using base64 = codecs::binary_to_text::basic_codec<
    codecs::base64::detail::encoding_traits<>>;
}
