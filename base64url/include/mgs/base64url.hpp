#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <mgs/codecs/base64url/decoder.hpp>
#include <mgs/codecs/base64url/detail/basic_codec_traits.hpp>
#include <mgs/codecs/base64url/encoder.hpp>
#include <mgs/codecs/base64url/nopad_decoder.hpp>
#include <mgs/codecs/base64url/nopad_encoder.hpp>
#include <mgs/codecs/basic_codec.hpp>

namespace mgs
{
inline namespace v1
{
using base64url = codecs::basic_codec<
    codecs::base64url::detail::basic_codec_traits<codecs::base64url::encoder,
                                                  codecs::base64url::decoder>>;

using base64url_nopad =
    codecs::basic_codec<codecs::base64url::detail::basic_codec_traits<
        codecs::base64url::nopad_encoder,
        codecs::base64url::nopad_decoder>>;
}
}
