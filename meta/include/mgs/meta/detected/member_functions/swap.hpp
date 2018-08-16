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
namespace member_functions
{
template <typename T, typename U>
using swap = decltype(std::declval<T>().swap(std::declval<U>()));
}
};
}
}
}
