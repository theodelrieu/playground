#pragma once

#include <type_traits>

#include <mgs/adapters/concepts/input_adapter.hpp>
#include <mgs/meta/concepts/iterator/iterable.hpp>

// template <typename T>
// concept IterableInputAdapter = requires(T const&) {
//    requires InputAdapter<T>;
//    requires Iterable<T>;
//  }

namespace mgs
{
inline namespace v1
{
namespace adapters
{
namespace concepts
{
template <typename T>
struct is_iterable_input_adapter
{
  static constexpr bool value = is_input_adapter<T>::value &&
                                meta::concepts::iterator::is_iterable<T>::value;
};
}
}
}
}
