#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/core/complete_type.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/types/difference_type.hpp>
#include <mgs/meta/detected/types/iterator_category.hpp>
#include <mgs/meta/detected/types/pointer.hpp>
#include <mgs/meta/detected/types/reference.hpp>
#include <mgs/meta/detected/types/value_type.hpp>
#include <mgs/meta/iterator_traits.hpp>

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
namespace detail
{
template <typename T>
struct is_iterator_traits_impl : std::false_type
{
};

template <typename T>
struct is_iterator_traits_impl<meta::iterator_traits<T>>
{
  using traits = meta::iterator_traits<T>;

  static constexpr auto value =
      is_detected<detected::types::value_type, traits>::value &&
      is_detected<detected::types::difference_type, traits>::value &&
      is_detected<detected::types::pointer, traits>::value &&
      is_detected<detected::types::iterator_category, traits>::value &&
      is_detected<detected::types::reference, traits>::value;
};

template <typename T>
struct is_iterator_traits_impl<std::iterator_traits<T>>
  : is_iterator_traits_impl<meta::iterator_traits<T>>
{
};
}

template <typename T>
struct is_iterator_traits : detail::is_iterator_traits_impl<T>
{
  using requirements = std::tuple<>;

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_iterator_traits::value, "T is not a valid IteratorTraits");
    return 1;
  }
};

template <typename T, typename = std::enable_if_t<is_iterator_traits<T>::value>>
using IteratorTraits = T;
}
};
}
}
}
