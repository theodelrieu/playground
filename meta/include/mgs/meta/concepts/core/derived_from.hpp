#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/detected.hpp>

#include <mgs/meta/concepts/object/regular.hpp>
#include <mgs/meta/void_t.hpp>

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
namespace detail
{
template <typename Derived, typename Base, typename = void>
struct is_derived_from_impl : std::false_type
{
};

// Derived must be a complete type.
template <typename Derived, typename Base>
struct is_derived_from_impl<Derived, Base, void_t<decltype(sizeof(Derived))>>
{
  static constexpr auto const value =
      std::is_base_of<Base, Derived>::value &&
      std::is_convertible<Derived const volatile*, Base const volatile*>::value;
};
}

template <typename Derived, typename Base>
struct is_derived_from
{
  static constexpr auto const value =
      detail::is_derived_from_impl<Derived, Base>::value;

  using requirements = std::tuple<>;

  struct static_assert_t
  {
    static_assert(value, "Derived is not derived from Base");
  };
};

template <typename Derived,
          typename Base,
          typename = std::enable_if_t<is_derived_from<Derived, Base>::value>>
using DerivedFrom = Derived;
}
};
}
}
}
