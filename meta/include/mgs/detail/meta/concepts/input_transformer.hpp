#pragma once

#include <type_traits>

#include <mgs/detail/meta/aliases.hpp>
#include <mgs/detail/meta/concepts/iterator.hpp>
#include <mgs/detail/meta/concepts/regular.hpp>
#include <mgs/detail/meta/concepts/sentinel.hpp>
#include <mgs/detail/meta/detected.hpp>

// template <typename T>
// concept InputTransformer = requires(T const& v, T& u) {
//  requires Regular<T>;
//  typename T::value_type;
//  typename T::difference_type;
//  typename T::underlying_iterator;
//  typename T::underlying_sentinel;
//  requires Constructible<T, T::underlying_iterator, T::underlying_sentinel>;
//  requires(typename T::difference_type n) {
//     v.get();
//     u.seek_forward(n);
//     requires { v.get() } -> value_type const&;
//     requires { u.seek_forward(n) } -> void;
//   }
// }

namespace mgs
{
inline namespace v1
{
namespace detail
{
template <typename T, typename = void>
struct is_input_transformer : std::false_type
{
};

template <typename T>
struct is_input_transformer<
    T,
    std::enable_if_t<is_regular<T>::value &&
                     is_detected<value_type_t, T>::value &&
                     is_detected<underlying_iterator_t, T>::value &&
                     is_detected<underlying_sentinel_t, T>::value &&
                     is_detected<difference_type_t, T>::value>>
{
private:
  using value_type = typename T::value_type;
  using difference_type = typename T::difference_type;

public:
  static auto constexpr value =
      std::is_constructible<T,
                            underlying_iterator_t<T>,
                            underlying_sentinel_t<T>>::value &&
      is_detected<get_function_t, T const&>::value &&
      is_detected_exact<void, seek_forward_function_t, T&, difference_type>::
          value;
};
}
}
}
