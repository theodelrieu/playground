#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/core/complete_type.hpp>
#include <mgs/meta/concepts/iterator/iterator_traits.hpp>
#include <mgs/meta/concepts/iterator/weakly_incrementable.hpp>
#include <mgs/meta/concepts/object/semiregular.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/dereference.hpp>
#include <mgs/meta/detected/operators/pre_increment.hpp>

// http://en.cppreference.com/w/cpp/concept/Iterator

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
namespace iterator
{
namespace detail
{
template <typename T, typename = void>
struct is_iterator_impl : std::false_type
{
};

template <typename T>
struct is_iterator_impl<T, std::enable_if_t<core::is_complete_type<T>::value>>
  : std::integral_constant<
        bool,
        is_iterator_traits<std::iterator_traits<T>>::value &&
            is_weakly_incrementable<T>::value &&
            is_detected<detected::operators::dereference, T&>::value>
{
};
}

template <typename T>
struct is_iterator : detail::is_iterator_impl<T>
{
  using requirements = std::tuple<is_iterator_traits<std::iterator_traits<T>>,
                                  is_weakly_incrementable<T>>;

  struct static_assert_t
  {
    static constexpr auto const has_dereference =
        is_detected<detected::operators::dereference, T&>::value;

    static constexpr int trigger_static_asserts()
    {
      static_assert(is_iterator::value, "T is not an Iterator");
      static_assert(has_dereference,
                    "Missing or invalid operator: '/* any */ operator*()'");
      return 0;
    }

    static constexpr auto _ = trigger_static_asserts();
  };
};

template <typename T, typename = std::enable_if_t<is_iterator<T>::value>>
using Iterator = T;
}
};
}
}
}
