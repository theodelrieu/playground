#pragma once

namespace mgs
{
inline namespace v1
{
namespace ranges
{
namespace detail
{
namespace detected
{
namespace types
{
template <typename T>
using underlying_sentinel = typename T::underlying_sentinel;
}
}
}
}
}
}