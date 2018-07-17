#pragma once

#include <type_traits>

#include <mgs/detail/meta/concepts/input_adapter.hpp>
#include <mgs/detail/meta/concepts/iterable.hpp>

// template <typename T>
// concept IterableInputAdapter = requires(T const&) {
//    requires InputAdapter<T>;
//    requires Iterable<T>;
//  }

namespace mgs
{
inline namespace v1
{
namespace detail
{
template <typename T>
struct is_iterable_input_adapter
{
  static constexpr bool value =
      is_input_adapter<T>::value && is_iterable<T>::value;
};
}
}
}
