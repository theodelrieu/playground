#pragma once

#include <utility>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace binary_to_text
{
namespace detail
{
namespace detected
{
namespace static_member_functions
{
template <typename T, typename... Args>
using find_char = decltype(T::find_char(std::declval<Args>()...));
}
}
}
}
}
}
}
