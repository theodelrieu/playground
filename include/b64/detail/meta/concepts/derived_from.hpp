#pragma once

#include <type_traits>

#include <b64/detail/meta/detected.hpp>

// http://en.cppreference.com/w/cpp/experimental/ranges/concepts/DerivedFrom

namespace b64
{
namespace detail
{
template <typename T, typename U>
struct is_derived_from
  : std::integral_constant<bool,
                           std::is_base_of<U, T>::value &&
                               std::is_convertible<std::remove_cv_t<T>*,
                                                   std::remove_cv_t<U>*>::value>
{
};
}
}
