#pragma once

#include <mgs/codecs/base64/decoder.hpp>
#include <mgs/codecs/base64/encoder.hpp>
#include <mgs/codecs/basic_codec.hpp>

namespace mgs
{
inline namespace v1
{
using base64 =
    codecs::basic_codec<codecs::base64::encoder, codecs::base64::decoder>;
}
}
