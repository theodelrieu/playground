#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <mgs/codecs/base16/detail/codec_traits.hpp>
#include <mgs/codecs/basic_codec.hpp>

namespace mgs
{
inline namespace v1
{
using base16 = codecs::basic_codec<codecs::base16::detail::codec_traits>;
}
}
