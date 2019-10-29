#pragma once

#include <type_traits>

namespace mgs
{
namespace codecs
{
namespace detected
{
namespace member_functions
{
template <typename T, typename... Args>
using max_remaining_size =
    decltype(std::declval<T>().max_remaining_size(std::declval<Args>()...));
}
}
}
}
