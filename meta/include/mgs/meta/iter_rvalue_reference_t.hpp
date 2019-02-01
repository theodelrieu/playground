#pragma once

#include <type_traits>
#include <utility>

#include <mgs/meta/detected/operators/dereference.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace detail
{
template <typename T, typename U = detected::operators::dereference<T>>
auto iter_move(T&& t)
    -> std::conditional_t<std::is_lvalue_reference<U>::value,
                          decltype(std::move(std::declval<U>())),
                          U>;
}

template <typename T>
using iter_rvalue_reference_t = decltype(detail::iter_move(std::declval<T&>()));
}
}
}
