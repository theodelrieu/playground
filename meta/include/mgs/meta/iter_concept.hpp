#pragma once

#include <iterator>
#include <type_traits>

#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/types/difference_type.hpp>
#include <mgs/meta/detected/types/iterator_category.hpp>
#include <mgs/meta/detected/types/iterator_concept.hpp>
#include <mgs/meta/detected/types/pointer.hpp>
#include <mgs/meta/detected/types/reference.hpp>
#include <mgs/meta/detected/types/value_type.hpp>
#include <mgs/meta/iter_traits.hpp>
#include <mgs/meta/iterator_traits.hpp>
#include <mgs/meta/void_t.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace detail
{
template <typename T, typename = void>
struct iter_concept_impl
{
};

template <typename T>
struct iter_concept_impl<T, void_t<detected::types::iterator_concept<T>>>
{
  using type = detected::types::iterator_concept<T>;
};

template <typename T>
struct iter_concept_impl<
    T,
    std::enable_if_t<
        !is_detected<detected::types::iterator_concept, T>::value &&
        is_detected<detected::types::iterator_category, T>::value>>
{
  using type = detected::types::iterator_category<T>;
};

template <typename T>
struct iter_concept_impl<
    T,
    std::enable_if_t<
        !is_detected<detected::types::iterator_concept, T>::value &&
        !is_detected<detected::types::iterator_category, T>::value &&
        // iterator_traits' primary template does not define any alias.
        // Therefore we must check that none is defined here.
        concepts::iterator::detail::is_iterator_traits_primary_template<
            meta::iterator_traits<T>>::value>>
{
  using type = std::random_access_iterator_tag;
};
}

template <typename T>
using iter_concept = typename detail::iter_concept_impl<iter_traits<T>>::type;
}
}
}
