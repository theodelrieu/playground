#pragma once

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace detected
{
namespace types
{
template <typename T>
using iterator_category = typename T::iterator_category;
}
}
}
}
}
