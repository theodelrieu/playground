#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <mgs/codecs/base64/decoder.hpp>
#include <mgs/codecs/base64/detail/codec_traits.hpp>
#include <mgs/codecs/base64/encoder.hpp>
#include <mgs/codecs/basic_codec.hpp>

namespace mgs
{
inline namespace v1
{
using base64 = codecs::basic_codec<codecs::base64::detail::codec_traits>;
}
}
