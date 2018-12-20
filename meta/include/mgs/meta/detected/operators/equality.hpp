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
namespace operators
{
template <typename T, typename U>
using equality = decltype(std::declval<T>() == std::declval<U>());
}
}
}
}
}