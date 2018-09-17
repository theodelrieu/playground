#pragma once

#include <type_traits>

#include <mgs/adapters/concepts/input_adapter.hpp>
#include <mgs/meta/call_std/begin.hpp>
#include <mgs/meta/concepts/iterator/iterable.hpp>
#include <mgs/meta/detected/types/iterator.hpp>

// template <typename T>
// concept IterableInputAdapter =
//   InputAdapter<T> &&
//   Iterable<T> &&
//   Same<T::iterator, result_of_begin<T>>;

namespace mgs
{
inline namespace v1
{
namespace adapters
{
namespace concepts
{
template <typename T, typename = void>
struct is_iterable_input_adapter : std::false_type
{
};

template <typename T>
struct is_iterable_input_adapter<
    T,
    std::enable_if_t<is_input_adapter<T>::value &&
                     meta::concepts::iterator::is_iterable<T>::value>>
{
  static constexpr bool value =
      meta::is_detected_exact<meta::result_of_begin<T>,
                              meta::detected::types::iterator,
                              T>::value;
};
}
}
}
}
