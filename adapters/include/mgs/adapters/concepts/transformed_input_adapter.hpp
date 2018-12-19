#pragma once

#include <cstddef>
#include <tuple>
#include <type_traits>

#include <mgs/adapters/detail/detected/member_functions/get.hpp>
#include <mgs/adapters/detail/detected/member_functions/read.hpp>
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

// clang-format off
//
// template <typename T, OutputIterator OI = typename T::value_type*>
// concept TransformedInputAdapter = Regular<T> &&
// requires(T const& v, T& u) {
//    requires Iterator<typename T::underlying_iterator>;
//    requires Sentinel<typename T::underlying_sentinel, typename T::underlying_iterator>;
//    typename T::value_type;
//    typename T::difference_type;
//    requires Constructible<T, typename T::underlying_iterator, typename T::underlying_sentinel>;
//    requires(typename T::difference_type n, OI out, std::size_t s) {
//      { u.read(out, n) } -> std::size_t;
//      { u.seek_forward(n) } -> void;
//      { v.get() } -> value_type const&;
//    }
// }
//
// clang-format on

namespace mgs
{
inline namespace v1
{
namespace adapters
{
namespace concepts
{
template <typename T,
          typename OutputIterator = std::add_pointer_t<
              meta::detected_t<meta::detected::types::value_type, T>>>
struct is_transformed_input_adapter
{
private:
  using value_type = meta::detected_t<meta::detected::types::value_type, T>;
  using difference_type =
      meta::detected_t<meta::detected::types::difference_type, T>;

  using lvalue_ref = std::add_lvalue_reference_t<T>;
  using lvalue_const_ref = std::add_lvalue_reference_t<std::add_const_t<T>>;

  using I = meta::detected_t<detail::detected::types::underlying_iterator, T>;
  using S = meta::detected_t<detail::detected::types::underlying_sentinel, T>;

  static auto constexpr const has_read_method =
      meta::is_detected_exact<std::size_t,
                              detail::detected::member_functions::read,
                              lvalue_ref,
                              OutputIterator,
                              std::size_t>::value;

  static auto constexpr const has_get_method =
      meta::is_detected_convertible<value_type const&,
                                    detail::detected::member_functions::get,
                                    lvalue_const_ref>::value;

  static auto constexpr const has_seek_forward_method =
      meta::is_detected_exact<void,
                              detail::detected::member_functions::seek_forward,
                              lvalue_ref,
                              difference_type>::value;

  static auto constexpr const is_constructible_from_iterator_sentinel =
      std::is_constructible<T, I, S>::value;

public:
  using requirements =
      std::tuple<meta::concepts::object::is_regular<T>,
                 meta::concepts::iterator::is_iterator<OutputIterator>>;

  static auto constexpr value =
      has_get_method && has_seek_forward_method && has_read_method &&
      is_constructible_from_iterator_sentinel &&
      meta::concepts::object::is_regular<T>::value &&
      meta::concepts::iterator::is_iterator<OutputIterator>::value &&
      meta::concepts::iterator::is_iterator<I>::value &&
      meta::concepts::iterator::is_sentinel<S, I>::value &&
      std::is_signed<difference_type>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not a TransformedInputAdapter");
    static_assert(
        has_get_method,
        "Invalid or missing function: 'T::value_type const& get() const'");
    static_assert(
        has_seek_forward_method,
        "Invalid or missing function: 'void seek_forward(T::difference_type)'");
    static_assert(has_read_method,
                  "Invalid or missing function: 'std::size_t "
                  "T::read(T::value_type*, std::size_t)'");
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

template <
    typename T,
    typename OutputIterator = std::add_pointer_t<
        meta::detected_t<meta::detected::types::value_type, T>>,
    std::enable_if_t<is_transformed_input_adapter<T, OutputIterator>::value,
                     int> = 0>
using TransformedInputAdapter = T;
}
}
}
}
