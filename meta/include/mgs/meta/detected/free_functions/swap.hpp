#pragma once

#include <utility>

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace detected
{
namespace free_functions
{
template <typename T, typename U>
using swap = decltype(swap(std::declval<T>(), std::declval<U>()));
}
};
}
}
}
