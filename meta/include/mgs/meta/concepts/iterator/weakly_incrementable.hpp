#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/core/complete_type.hpp>
#include <mgs/meta/concepts/object/semiregular.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/post_increment.hpp>
#include <mgs/meta/detected/operators/pre_increment.hpp>

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
struct is_weakly_incrementable_impl : std::false_type
{
};

template <typename T>
struct is_weakly_incrementable_impl<
    T,
    std::enable_if_t<core::is_complete_type<T>::value>>
{
  static constexpr auto const value =
      object::is_semiregular<T>::value &&
      is_detected_exact<T&, detected::operators::pre_increment, T&>::value &&
      is_detected<detected::operators::post_increment, T&>::value;
};
}

template <typename T>
struct is_weakly_incrementable : detail::is_weakly_incrementable_impl<T>
{
  using requirements = std::tuple<object::is_semiregular<T>>;

  struct static_assert_t
  {
    static constexpr auto const has_pre_increment =
        is_detected_exact<T&, detected::operators::pre_increment, T&>::value;

    static constexpr auto const has_post_increment =
        is_detected<detected::operators::post_increment, T&>::value;

    static constexpr int trigger_static_asserts()
    {
      static_assert(is_weakly_incrementable::value,
                    "T is not WeaklyIncrementable");
      static_assert(
          has_pre_increment,
          "Missing or invalid operator: 'T& operator++()'");
      static_assert(
          has_post_increment,
          "Missing or invalid operator: '/* any */ operator++(int)'");
      return 0;
    }

    static constexpr auto _ = trigger_static_asserts();
  };
};

template <typename T,
          typename = std::enable_if_t<is_weakly_incrementable<T>::value>>
using WeaklyIncrementable = T;
}
};
}
}
}
