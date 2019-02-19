#pragma once

#include <iterator>
#include <type_traits>

#include <mgs/meta/concepts/iterator/detail/iterator_traits.hpp>
#include <mgs/meta/iterator_traits.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace detail
{
template <typename T, typename = void>
struct iter_traits_impl
{
};

template <typename T>
struct iter_traits_impl<
    T,
    std::enable_if_t<
        concepts::iterator::detail::is_iterator_traits_primary_template<
            meta::iterator_traits<T>>::value>>
{
  using type = T;
};

template <typename T>
struct iter_traits_impl<T,
                        std::enable_if_t<concepts::iterator::detail::is_iterator_traits<
                            // use our own iterator_traits for
                            // SFINAE-correctness purposes
                            meta::iterator_traits<T>>::value>>
{
  // but expose the std one when SFINAE checks were passed
  using type = std::iterator_traits<T>;
};
}

// Helper defined in the standard to implement ITER_CONCEPT
template <typename T>
using iter_traits = typename detail::iter_traits_impl<T>::type;
}
}
}
