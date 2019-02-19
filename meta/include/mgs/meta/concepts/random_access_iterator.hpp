#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/strict_totally_ordered.hpp>
#include <mgs/meta/concepts/derived_from.hpp>
#include <mgs/meta/concepts/bidirectional_iterator.hpp>
#include <mgs/meta/concepts/sized_sentinel.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/addition.hpp>
#include <mgs/meta/detected/operators/addition_assignment.hpp>
#include <mgs/meta/detected/operators/array_subscript.hpp>
#include <mgs/meta/detected/operators/substraction.hpp>
#include <mgs/meta/detected/operators/substraction_assignment.hpp>
#include <mgs/meta/detected/types/reference.hpp>
#include <mgs/meta/iter_concept.hpp>
#include <mgs/meta/iter_difference_t.hpp>
#include <mgs/meta/iter_reference_t.hpp>

// https://en.cppreference.com/w/cpp/experimental/ranges/RandomAccessIterator

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
template <typename T>
struct is_random_access_iterator
{
private:
  using lvalue_ref = std::add_lvalue_reference_t<T>;
  using difference_type = detected_t<iter_difference_t, T>;

  static constexpr auto const has_correct_category =
      is_derived_from<detected_t<meta::iter_concept, T>,
                      std::random_access_iterator_tag>::value;

  static constexpr auto const has_addition_assignment =
      is_detected_exact<lvalue_ref,
                        detected::operators::addition_assignment,
                        lvalue_ref,
                        difference_type const>::value;

  static constexpr auto const has_addition_t_dt =
      is_detected_exact<T,
                        detected::operators::addition,
                        T const,
                        difference_type const>::value;

  static constexpr auto const has_addition_dt_t =
      is_detected_exact<T,
                        detected::operators::addition,
                        difference_type const,
                        T const>::value;

  static constexpr auto const has_substraction =
      is_detected_exact<T,
                        detected::operators::substraction,
                        T const,
                        difference_type const>::value;

  static constexpr auto const has_substraction_assignment =
      is_detected_exact<lvalue_ref,
                        detected::operators::substraction_assignment,
                        lvalue_ref,
                        difference_type const>::value;

  static constexpr auto const has_array_subscript =
      is_detected_exact<detected_t<iter_reference_t, T>,
                        detected::operators::array_subscript,
                        T const,
                        difference_type const>::value;

public:
  using requirements = std::tuple<is_bidirectional_iterator<T>,
                                  is_strict_totally_ordered<T>,
                                  is_sized_sentinel<T, T>>;

  static auto constexpr value =
      is_bidirectional_iterator<T>::value &&
      is_strict_totally_ordered<T>::value &&
      is_sized_sentinel<T, T>::value && has_correct_category &&
      has_addition_assignment && has_addition_t_dt && has_addition_dt_t &&
      has_substraction && has_substraction_assignment && has_array_subscript;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not a RandomAccessIterator");
    static_assert(has_correct_category,
                  "Iterator category tag must derive from "
                  "std::random_access_iterator_tag");
    static_assert(has_addition_assignment,
                  "Invalid or missing operator: 'T& "
                  "operator+=(std::iterator_traits<T>::difference_type)'");
    static_assert(has_addition_t_dt,
                  "Invalid or missing operator: 'T operator+(T const&, "
                  "std::iterator_traits<T>::difference_type)'");
    static_assert(has_addition_dt_t,
                  "Invalid or missing operator: 'T "
                  "operator+(std::iterator_traits<T>::difference_type, T "
                  "const&)'");
    static_assert(has_substraction_assignment,
                  "Invalid or missing operator: 'T "
                  "operator-=(std::iterator_traits<T>::difference_type)'");
    static_assert(has_substraction,
                  "Invalid or missing operator: 'T operator-(T const&, "
                  "std::iterator_traits<T>::difference_type)'");
    static_assert(has_array_subscript,
                  "Invalid or missing operator: "
                  "'std::iterator_traits<T>::reference "
                  "operator[](std::iterator_traits<T>::difference_type)'");
    return 1;
  }
};
}

template <typename T,
          typename =
              std::enable_if_t<concepts::is_random_access_iterator<T>::value>>
using RandomAccessIterator = T;
}
}
}