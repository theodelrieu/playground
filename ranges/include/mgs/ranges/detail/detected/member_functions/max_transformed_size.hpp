#pragma once

#include <utility>

namespace mgs
{
inline namespace v1
{
namespace ranges
{
namespace detail
{
namespace detected
{
namespace member_functions
{
template <typename T, typename... Args>
using max_transformed_size =
    decltype(std::declval<T>().max_transformed_size(std::declval<Args>()...));
}
}
}
}
}
}
