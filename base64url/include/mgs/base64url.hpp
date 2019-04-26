#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <mgs/base64url/detail/basic_encoding_traits.hpp>
#include <mgs/binary_to_text/basic_codec.hpp>

namespace mgs
{
inline namespace v1
{
using base64url =
    binary_to_text::basic_codec<detail::basic_base64url_encoding_traits<
        binary_to_text::padding_policy::required>>;

using base64url_nopad =
    binary_to_text::basic_codec<detail::basic_base64url_encoding_traits<
                                    binary_to_text::padding_policy::none>,
                                detail::basic_base64url_encoding_traits<
                                    binary_to_text::padding_policy::optional>>;
}
}
