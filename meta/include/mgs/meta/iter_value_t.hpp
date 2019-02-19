#pragma once

#include <mgs/meta/concepts/detail/iterator_traits.hpp>
#include <mgs/meta/detected/types/value_type.hpp>
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
struct iter_value<
    T,
    std::enable_if_t<
        !concepts::detail::is_iterator_traits_primary_template<
            meta::iterator_traits<T>>::value &&
        is_detected<detected::types::value_type, T>::value>>
{
  using value_type = detected::types::value_type<T>;
};
}

template <typename T>
using iter_value_t = typename detail::iter_value<T>::value_type;
}
}
}
