#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/detected.hpp>

#include <mgs/meta/concepts/core/complete_type.hpp>
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
namespace detail
{
template <typename Derived, typename Base, typename = void>
struct is_derived_from_impl : std::false_type
{
};

// Derived must be a complete type.
template <typename Derived, typename Base>
struct is_derived_from_impl<Derived,
                            Base,
                            std::enable_if_t<is_complete_type<Derived>::value>>
{
  static constexpr auto const value =
      std::is_base_of<Base, Derived>::value &&
      std::is_convertible<Derived const volatile*, Base const volatile*>::value;
};
}

template <typename Derived, typename Base>
struct is_derived_from : detail::is_derived_from_impl<Derived, Base>
{
  using requirements = std::tuple<>;

  struct static_assert_t
  {
    static constexpr int trigger()
    {
      static_assert(is_derived_from::value, "Derived is not derived from Base");
      return 1;
    }
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
