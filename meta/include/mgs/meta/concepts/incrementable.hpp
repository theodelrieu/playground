#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/complete_type.hpp>
#include <mgs/meta/concepts/weakly_incrementable.hpp>
#include <mgs/meta/concepts/regular.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/post_increment.hpp>

// https://en.cppreference.com/w/cpp/experimental/ranges/Incrementable

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
template <typename T>
struct is_incrementable
{
  using requirements = std::tuple<is_regular<T>, is_weakly_incrementable<T>>;
  using lvalue_ref = std::add_lvalue_reference_t<T>;

  static constexpr auto const has_post_increment =
      is_detected_exact<T, detected::operators::post_increment, lvalue_ref>::
          value;

  static constexpr auto const value = is_regular<T>::value &&
                                      has_post_increment &&
                                      is_weakly_incrementable<T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not Incrementable");
    static_assert(has_post_increment,
                  "Invalid or missing operator: 'T operator++(int)'");
    return 1;
  }
};
}

template <typename T,
          typename = std::enable_if_t<concepts::is_incrementable<T>::value>>
using Incrementable = T;
}
}
}