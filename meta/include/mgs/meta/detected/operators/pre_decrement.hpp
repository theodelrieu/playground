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
template <typename T>
using pre_decrement = decltype(--std::declval<T>());
}
};
}
}
}
