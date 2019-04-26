#pragma once

#include <utility>

namespace mgs
{
namespace detected
{
namespace static_member_functions
{
template <typename T, typename... Args>
using make_encoder = decltype(T::make_encoder(std::declval<Args>()...));
}
}
}
