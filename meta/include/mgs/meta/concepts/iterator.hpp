#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/complete_type.hpp>
#include <mgs/meta/concepts/dereferenceable.hpp>
#include <mgs/meta/concepts/weakly_incrementable.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/dereference.hpp>

// https://en.cppreference.com/w/cpp/experimental/ranges/Iterator

namespace mgs
{
namespace meta
{
namespace concepts
{
template <typename T>
struct is_iterator
{
  using requirements =
      std::tuple<is_dereferenceable<T>, is_weakly_incrementable<T>>;

  static constexpr auto const value =
      is_weakly_incrementable<T>::value && is_dereferenceable<T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not an Iterator");
    return 1;
  }
};

template <typename T>
constexpr auto is_iterator_v = is_iterator<T>::value;
}

template <typename T,
          typename = std::enable_if_t<concepts::is_iterator<T>::value>>
using Iterator = T;
}
}
