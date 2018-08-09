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
template <typename T>
using post_decrement = decltype(std::declval<T>()--);
}
}
namespace operator_aliases = aliases::operators;
}
}
}
