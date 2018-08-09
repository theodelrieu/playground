#pragma once

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace aliases
{
namespace types
{
template <typename T>
using iterator_category = typename T::iterator_category;
}
}
namespace type_aliases = aliases::types;
}
}
}
