#pragma once

#include <iterator>
#include <type_traits>

#include <mgs/meta/concepts/comparison/strict_totally_ordered.hpp>
#include <mgs/meta/concepts/core/derived_from.hpp>
#include <mgs/meta/concepts/iterator/bidirectional_iterator.hpp>
#include <mgs/meta/concepts/iterator/sized_sentinel.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/addition.hpp>
#include <mgs/meta/detected/operators/addition_assignment.hpp>
#include <mgs/meta/detected/operators/array_subscript.hpp>
#include <mgs/meta/detected/operators/substraction.hpp>
#include <mgs/meta/detected/operators/substraction_assignment.hpp>
#include <mgs/meta/detected/types/reference.hpp>

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
template <typename T, typename = void>
struct is_random_access_iterator : std::false_type
{
};

template <typename T>
struct is_random_access_iterator<
    T,
    std::enable_if_t<
        is_bidirectional_iterator<T>::value &&
        core::is_derived_from<detected_t<detected::types::iterator_category,
                                         std::iterator_traits<T>>,
                              std::random_access_iterator_tag>::value>>
{
private:
  using difference_type =
      detected::types::difference_type<std::iterator_traits<T>>;

public:
  static auto constexpr value =
      comparison::is_strict_totally_ordered<T>::value &&
      is_sized_sentinel<T, T>::value &&
      is_detected_exact<T&,
                        detected::operators::addition_assignment,
                        T&,
                        difference_type const>::value &&
      is_detected_exact<T,
                        detected::operators::addition,
                        T const,
                        difference_type const>::value &&
      is_detected_exact<T,
                        detected::operators::addition,
                        difference_type const,
                        T const>::value &&
      is_detected_exact<T,
                        detected::operators::substraction,
                        T const,
                        difference_type const>::value &&
      is_detected_exact<T&,
                        detected::operators::substraction_assignment,
                        T&,
                        difference_type const>::value &&
      std::is_convertible<
          detected_t<detected::operators::array_subscript,
                     T const,
                     difference_type const>,
          detected::types::reference<std::iterator_traits<T>>>::value &&
      std::is_convertible<
          detected_t<detected::operators::array_subscript,
                     T,
                     difference_type const>,
          detected::types::reference<std::iterator_traits<T>>>::value;
};
}
}
namespace iterator_concepts = concepts::iterator;
}
}
}
