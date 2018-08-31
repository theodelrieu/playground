#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/detected.hpp>

#include <mgs/meta/concepts/object/regular.hpp>

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
{
  static constexpr auto const value =
      std::is_base_of<U, T>::value &&
      std::is_convertible<T const volatile*, U const volatile*>::value;

  using requirements = std::tuple<>;

  struct static_assert_t
  {
    static_assert(value, "T is not derived from U");
  };
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
