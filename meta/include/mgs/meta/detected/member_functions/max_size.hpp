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
using max_size = decltype(std::declval<T>().max_size());
}
};
}
}
}
