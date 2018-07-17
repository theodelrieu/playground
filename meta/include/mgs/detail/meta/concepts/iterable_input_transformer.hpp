#pragma once

#include <type_traits>

#include <mgs/detail/meta/concepts/iterable.hpp>
#include <mgs/detail/meta/concepts/input_transformer.hpp>

// template <typename T>
// concept IterableInputTransformer = requires(T const&) {
//    requires InputTransformer<T>;
//    requires Iterable<T>;
//  }

namespace mgs
{
inline namespace v1
{
namespace detail
{
template <typename T>
struct is_iterable_input_transformer
{
  static constexpr bool value =
      is_input_transformer<T>::value && is_iterable<T>::value;
};
}
}
}
