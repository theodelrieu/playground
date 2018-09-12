#pragma once

#include <type_traits>
#include <utility>

#include <mgs/meta/detected.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace detected
{
namespace operators
{
namespace detail
{
template <typename T, typename U>
using substraction_assignment =
    decltype(std::declval<T>() -= std::declval<U>());

// Workaround a GCC bug with operator-=(void*, void*) in unevaluated context
template <typename T,
          typename U,
          typename uncvT = std::remove_cv_t<std::remove_reference_t<T>>,
          typename uncvU = std::remove_cv_t<std::remove_reference_t<U>>,
          typename = void>
struct substraction_assignment_impl;

template <typename T, typename U, typename uncvT, typename uncvU>
struct substraction_assignment_impl<
    T,
    U,
    uncvT,
    uncvU,
    std::enable_if_t<!std::is_same<uncvT, void*>::value &&
                     !std::is_same<uncvU, void*>::value>>
{
  using type = detected_t<substraction_assignment, T, U>;
};

template <typename T, typename U>
struct substraction_assignment_impl<T, U, void*, void*>
{
};
}

template <typename T, typename U>
using substraction_assignment =
    typename detail::substraction_assignment_impl<T, U>::type;
}
};
}
}
}
