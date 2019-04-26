#pragma once

#include <utility>

namespace mgs
{
namespace binary_to_text
{
namespace detected
{
namespace static_member_functions
{
template <typename T, typename... Args>
using index_of = decltype(T::index_of(std::declval<Args>()...));
}
}
}
}
