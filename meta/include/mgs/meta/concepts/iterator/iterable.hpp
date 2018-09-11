#pragma once

#include <type_traits>

#include <mgs/meta/call_std/begin.hpp>
#include <mgs/meta/call_std/end.hpp>
#include <mgs/meta/concepts/iterator/iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/detected.hpp>

// template <typename T>
// concept Iterable = requires(T& a) {
//  requires { begin(a) } -> Iterator
//  requires { end(a) } -> Sentinel<Iterator>
// }
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
template <typename T>
struct is_iterable
{
private:
  using t_ref = std::add_lvalue_reference_t<T>;
  using Iterator = detected_t<result_of_begin, t_ref>;
  using Sentinel = detected_t<result_of_end, t_ref>;

  static constexpr auto const has_begin = !std::is_same<Iterator, nonesuch>::value;
  static constexpr auto const has_end = !std::is_same<Sentinel, nonesuch>::value;

  static constexpr auto const is_iterator_begin = is_iterator<Iterator>::value;
  static constexpr auto const is_sentinel_end =
      is_sentinel<Sentinel, Iterator>::value;

public:
  static constexpr bool value = is_iterator_begin && is_sentinel_end;

  // do not trigger loads of static asserts when no function begin/end is found...
  using requirements = decltype(std::tuple_cat(
      std::conditional_t<has_begin,
                         std::tuple<is_iterator<Iterator>>,
                         std::tuple<>>{},
      std::conditional_t<has_begin && has_end,
                         std::tuple<is_sentinel<Sentinel, Iterator>>,
                         std::tuple<>>{}));

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not an Iterable");
    static_assert(
        has_begin,
        "There is not an available 'begin' member/free function for T");
    static_assert(
        has_end,
        "There is not an available 'end' member/free function for T");

    // only trigger if there is a begin function
    static_assert(!has_begin || is_iterator_begin,
                  "Invalid 'begin' member/free function for T: does not return "
                  "an Iterator");

    // only trigger if there is a begin and an end function.
    static_assert(
        (!has_begin || is_iterator_begin) || !has_end || is_sentinel_end,
        "Invalid 'end' member/free function for T: does not return "
        "a Sentinel<Iterator>");
    return 1;
  }
};

template <typename T, typename = std::enable_if_t<is_iterable<T>::value>>
using Iterable = T;
}
};
}
}
}
