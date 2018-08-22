#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <mgs/codecs/base32hex/detail/codec_traits.hpp>
#include <mgs/codecs/basic_codec.hpp>

namespace mgs
{
inline namespace v1
{
using base32hex = codecs::basic_codec<codecs::base32hex::detail::codec_traits>;
}
}
