#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/adapters/detail/detected/member_functions/get.hpp>
#include <mgs/adapters/detail/detected/member_functions/seek_forward.hpp>
#include <mgs/adapters/detail/detected/types/underlying_iterator.hpp>
#include <mgs/adapters/detail/detected/types/underlying_sentinel.hpp>
#include <mgs/meta/concepts/iterator/iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/concepts/object/regular.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/types/difference_type.hpp>
#include <mgs/meta/detected/types/iterator_category.hpp>
#include <mgs/meta/detected/types/reference.hpp>
#include <mgs/meta/detected/types/value_type.hpp>

// template <typename T>
// concept InputAdapter = requires(T const& v, T& u) {
//  requires Regular<T>;
//  requires Iterator<typename T::underlying_iterator>;
//  requires Sentinel<typename T::underlying_sentinel, typename T::underlying_iterator>;
//  typename T::value_type;
//  typename T::difference_type;
//  requires Constructible<T, typename T::underlying_iterator, typename T::underlying_sentinel>;
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
namespace adapters
{
namespace concepts
{
template <typename T>
struct is_input_adapter
{
private:
  using value_type = meta::detected_t<meta::detected::types::value_type, T>;
  using difference_type =
      meta::detected_t<meta::detected::types::difference_type, T>;

  using I = meta::detected_t<detail::detected::types::underlying_iterator, T>;
  using S = meta::detected_t<detail::detected::types::underlying_sentinel, T>;

  static auto constexpr const has_get_method =
      meta::is_detected_convertible<value_type const&,
                                    detail::detected::member_functions::get,
                                    T const&>::value;

  static auto constexpr const has_seek_forward_method =
      meta::is_detected_exact<void,
                              detail::detected::member_functions::seek_forward,
                              T&,
                              difference_type>::value;

  static auto constexpr const is_constructible_from_iterator_sentinel =
      std::is_constructible<T, I, S>::value;

public:
  using requirements = std::tuple<meta::concepts::object::is_regular<T>>;

  static auto constexpr value =
      has_get_method && has_seek_forward_method &&
      is_constructible_from_iterator_sentinel &&
      meta::concepts::object::is_regular<T>::value &&
      meta::concepts::iterator::is_iterator<I>::value &&
      meta::concepts::iterator::is_sentinel<S, I>::value &&
      std::is_signed<difference_type>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not an InputAdapter");
    static_assert(
        has_get_method,
        "Missing or invalid function: 'T::value_type const& get() const'");
    static_assert(
        has_seek_forward_method,
        "Missing or invalid function: 'void seek_forward(T::difference_type)'");
    static_assert(is_constructible_from_iterator_sentinel,
                  "T is not Constructible from Iterator/Sentinel pair");
    static_assert(std::is_signed<difference_type>::value,
                  "T::difference_type must be a signed integral type");
    static_assert(meta::concepts::iterator::is_iterator<I>::value,
                  "T::underlying_iterator must be an Iterator");
    static_assert(
        meta::concepts::iterator::is_sentinel<S, I>::value,
        "T::underlying_sentinel must be a Sentinel<T::underlying_iterator>");
    return 1;
  }
};
}
}
}
}
