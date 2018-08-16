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
template <typename T>
using empty = decltype(std::declval<T>().empty());
}
};
}
}
}
