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
using size = decltype(std::declval<T>().size());
}
};
}
}
}
