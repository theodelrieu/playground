#pragma once

namespace mgs
{
inline namespace v1
{
namespace adapters
{
namespace detail
{
namespace detected
{
namespace types
{
template <typename T>
using underlying_iterator = typename T::underlying_iterator;
}
}
}
}
}
}