#pragma once

#include <mgs/meta/concepts/iterator/iterator_traits.hpp>
#include <mgs/meta/iterator_traits.hpp>
#include <mgs/meta/readable_traits.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace detail
{
template <typename T, typename = void>
struct iter_value : readable_traits<T>
{
};

template <typename T>
struct iter_value<T,
                  std::enable_if_t<concepts::iterator::is_iterator_traits<
                      meta::iterator_traits<T>>::value>>
  : meta::iterator_traits<T>
{
};
}

template <typename T>
using iter_value_t = typename detail::iter_value<T>::value_type;
}
}
}
