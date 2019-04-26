#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/complete_type.hpp>
#include <mgs/meta/concepts/movable.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/post_increment.hpp>
#include <mgs/meta/detected/operators/pre_increment.hpp>
#include <mgs/meta/iter_difference_t.hpp>
#include <mgs/meta/void_t.hpp>

namespace mgs
{
namespace detail
{
// Before C++20, those three iterators define difference_type as void
// and thus they do not model WeaklyIncrementable.
template <typename T, typename = void>
struct is_insert_iterator : std::false_type
{
};

template <typename T>
struct is_insert_iterator<
    std::back_insert_iterator<T>,
    // instantiate the iterator to ensure T is not some
    // weird type (e.g. void)
    meta::void_t<typename std::back_insert_iterator<T>::container_type>>
  : std::true_type
{
};

template <typename T>
struct is_insert_iterator<
    std::front_insert_iterator<T>,
    meta::void_t<typename std::front_insert_iterator<T>::container_type>>
  : std::true_type
{
};

template <typename T>
struct is_insert_iterator<
    std::insert_iterator<T>,
    meta::void_t<typename std::insert_iterator<T>::container_type>>
  : std::true_type
{
};
}

namespace meta
{
namespace concepts
{
template <typename T>
struct is_weakly_incrementable
{
private:
  using lvalue_ref = std::add_lvalue_reference_t<T>;
  using difference_type_t = detected_t<iter_difference_t, T>;

  static constexpr auto const has_pre_increment =
      is_detected_exact<lvalue_ref,
                        detected::operators::pre_increment,
                        lvalue_ref>::value;

  static constexpr auto const has_post_increment =
      is_detected<detected::operators::post_increment, lvalue_ref>::value;

  static constexpr auto const has_signed_difference_type =
      detail::is_insert_iterator<T>::value ||
      (std::is_signed<difference_type_t>::value &&
       std::is_integral<difference_type_t>::value);

public:
  using requirements = std::tuple<is_movable<T>>;

  static constexpr auto const value = is_movable<T>::value &&
                                      has_pre_increment && has_post_increment &&
                                      has_signed_difference_type;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not WeaklyIncrementable");
    static_assert(has_pre_increment,
                  "Invalid or missing operator: 'T& operator++()'");
    static_assert(has_post_increment,
                  "Invalid or missing operator: '/* any */ operator++(int)'");
    static_assert(has_signed_difference_type,
                  "iter_difference_t<T> must be a SignedIntegral");
    return 1;
  }
};

template <typename T>
constexpr auto is_weakly_incrementable_v = is_weakly_incrementable<T>::value;
}

template <typename T,
          typename =
              std::enable_if_t<concepts::is_weakly_incrementable<T>::value>>
using WeaklyIncrementable = T;
}
}
