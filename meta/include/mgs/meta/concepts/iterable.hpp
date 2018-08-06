#pragma once

#include <type_traits>

#include <mgs/meta/can_call_std.hpp>
#include <mgs/meta/concepts/iterator.hpp>
#include <mgs/meta/concepts/sentinel.hpp>
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
CAN_CALL_STD_FUNC_IMPL(begin)
CAN_CALL_STD_FUNC_IMPL(end)

template <typename T, typename = void>
struct is_iterable : std::false_type
{
};

template <typename T>
struct is_iterable<
    T,
    std::enable_if_t<can_call_begin<T&>::value && can_call_end<T&>::value>>
{
private:
  // result_of_* are not SFINAE-friendly, hence the enable_if above.
  using Iterator = result_of_begin_t<T&>;
  using Sentinel = result_of_end_t<T&>;

public:
  static constexpr bool value =
      is_iterator<Iterator>::value && is_sentinel<Sentinel, Iterator>::value;
};
}
}
}
