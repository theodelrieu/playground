#pragma once

#include <type_traits>

#include <mgs/detail/meta/concepts/iterable.hpp>
#include <mgs/detail/meta/concepts/input_transformer.hpp>

// concept IterableInputTransformer = requires(T const&) {
//    requires InputTransformer<T>;
//    requires Iterable<T>;
//  }

namespace mgs
{
namespace detail
{
template <typename T,
          typename Iterator,
          typename Sentinel = Iterator>
struct is_iterable_input_transformer
{
  static constexpr bool value =
      is_input_transformer<T, Iterator, Sentinel>::value &&
      is_iterable<T>::value;
};
}
}
