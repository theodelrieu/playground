#pragma once

#include <utility>

namespace mgs
{
namespace concepts
{
namespace detected
{
namespace static_member_functions
{
template <typename T, typename... Args>
using make_decoder = decltype(T::make_decoder(std::declval<Args>()...));
}
}
}
}
