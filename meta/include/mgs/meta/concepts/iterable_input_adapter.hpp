#pragma once

#include <type_traits>

#include <mgs/meta/concepts/input_adapter.hpp>
#include <mgs/meta/concepts/iterable.hpp>

// template <typename T>
// concept IterableInputAdapter = requires(T const&) {
//    requires InputAdapter<T>;
//    requires Iterable<T>;
//  }

namespace mgs
{
namespace meta
{
inline namespace v1
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
