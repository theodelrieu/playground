#pragma once

#include <type_traits>

#include <mgs/meta/call_std/begin.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
template <typename T>
using iterator_t = result_of_begin<std::add_lvalue_reference_t<T>>;
}
}
}
