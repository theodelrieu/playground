#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <mgs/codecs/base32hex/detail/encoding_traits.hpp>
#include <mgs/codecs/binary_to_text/basic_codec.hpp>

namespace mgs
{
using base32hex = codecs::binary_to_text::basic_codec<
    codecs::base32hex::detail::encoding_traits<>>;
}
