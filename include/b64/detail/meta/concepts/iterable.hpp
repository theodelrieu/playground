#pragma once

#include <type_traits>

#include <b64/detail/meta/can_call_std.hpp>
#include <b64/detail/meta/concepts/iterator.hpp>
#include <b64/detail/meta/concepts/sentinel.hpp>
#include <b64/detail/meta/detected.hpp>

// concept Iterable = requires(T& a) {
//  requires { begin(a) } -> Iterator
//  requires { end(a) } -> Sentinel<Iterator>
// }
namespace b64
{
CAN_CALL_STD_FUNC_IMPL(begin)
CAN_CALL_STD_FUNC_IMPL(end)

namespace detail
{
template <typename T, typename = void>
struct is_iterable : std::false_type
{
};

template <typename T>
struct is_iterable<T,
                   std::enable_if_t<detail2::can_call_begin<T&>::value &&
                                    detail2::can_call_end<T&>::value>>
{
  private:
    // result_of_* are not SFINAE-friendly, hence the enable_if above.
    using Iterator = typename detail2::result_of_begin<T&>::type;
    using Sentinel = typename detail2::result_of_end<T&>::type;

  public:
    static constexpr bool value =
        is_iterator<Iterator>::value && is_sentinel<Sentinel, Iterator>::value;
};
}
}
