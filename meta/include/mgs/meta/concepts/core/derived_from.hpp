#pragma once

#include <type_traits>

#include <mgs/meta/detected.hpp>

// http://en.cppreference.com/w/cpp/experimental/ranges/concepts/DerivedFrom

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
namespace core
{
template <typename T, typename U>
struct is_derived_from
  : std::integral_constant<bool,
                           std::is_base_of<U, T>::value &&
                               std::is_convertible<std::remove_cv_t<T>*,
                                                   std::remove_cv_t<U>*>::value>
{
};

template <typename T,
          typename U,
          typename = std::enable_if_t<is_derived_from<T, U>::value>>
using DerivedFrom = T;
}
};
}
}
}
