#pragma once

#include <type_traits>

#include <mgs/meta/call_std/end.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
template <typename T>
using sentinel_t = result_of_end<std::add_lvalue_reference_t<T>>;
}
}
}
