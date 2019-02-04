#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/core/complete_type.hpp>
#include <mgs/meta/concepts/core/dereferenceable.hpp>
#include <mgs/meta/concepts/iterator/weakly_incrementable.hpp>
#include <mgs/meta/concepts/object/semiregular.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/dereference.hpp>

// https://en.cppreference.com/w/cpp/experimental/ranges/iterator/Iterator

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
template <typename T>
struct is_iterator
{
  using requirements =
      std::tuple<core::is_dereferenceable<T>, is_weakly_incrementable<T>>;

  static constexpr auto const value =
      is_weakly_incrementable<T>::value && core::is_dereferenceable<T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not an Iterator");
    return 1;
  }
};

template <typename T, typename = std::enable_if_t<is_iterator<T>::value>>
using Iterator = T;
}
};
}
}
}
