#pragma once

#include <utility>

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace aliases
{
namespace operators
{
template <typename T, typename U>
using array_subscript = decltype(std::declval<T>()[std::declval<U>()]);
}
}
namespace operator_aliases = aliases::operators;
}
}
}
