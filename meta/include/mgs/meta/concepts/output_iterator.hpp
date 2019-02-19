#pragma once

#include <tuple>
#include <type_traits>
#include <utility>

#include <mgs/meta/concepts/iterator.hpp>
#include <mgs/meta/concepts/writable.hpp>
#include <mgs/meta/detected.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
namespace detail
{
template <typename I, typename T>
auto _output_iterator_requirement(I i, T&& t)
    -> decltype(*i++ = std::forward<T>(t));

template <typename I, typename T>
using output_iterator_requirement = decltype(
    _output_iterator_requirement(std::declval<I>(), std::declval<T>()));
}

template <typename I, typename T>
struct is_output_iterator
{
  using requirements = std::tuple<is_iterator<I>, is_writable<I, T>>;

  static constexpr auto const value =
      is_iterator<I>::value && is_writable<I, T>::value &&
      is_detected<detail::output_iterator_requirement, I, T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "I is not an OutputIterator<T>");
    return 1;
  }
};
}

template <typename I,
          typename T,
          typename =
              std::enable_if_t<concepts::is_output_iterator<I, T>::value>>
using OutputIterator = I;
}
}
}