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
using pre_increment = decltype(++std::declval<T>());
}
}
namespace operator_aliases = aliases::operators;
}
}
}
