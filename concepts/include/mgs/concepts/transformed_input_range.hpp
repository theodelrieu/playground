#pragma once

#include <cstddef>
#include <tuple>
#include <type_traits>

#include <mgs/concepts/detail/detected/types/underlying_iterator.hpp>
#include <mgs/concepts/detail/detected/types/underlying_sentinel.hpp>
#include <mgs/concepts/iterable.hpp>
#include <mgs/meta/call_std/begin.hpp>
#include <mgs/meta/concepts/core/constructible.hpp>
#include <mgs/meta/concepts/iterator/iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/concepts/object/semiregular.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/types/iterator.hpp>
#include <mgs/meta/detected/types/iterator_category.hpp>
#include <mgs/meta/detected/types/reference.hpp>
#include <mgs/meta/detected/types/value_type.hpp>

// clang-format off
//
// template <typename T>
// concept TransformedInputRange = std::Semiregular<T> &&
//   Iterable<T> &&
//   requires {
//      Iterator<typename T::underlying_iterator>;
//      Sentinel<typename T::underlying_sentinel, typename T::underlying_iterator>;
//      typename T::value_type;
//      Same<typename T::iterator, result_of_begin<T>>;
//      Constructible<T, typename T::underlying_iterator, typename T::underlying_sentinel>;
//   };
//
// clang-format on

namespace mgs
{
inline namespace v1
{
namespace concepts
{
template <typename T>
struct is_transformed_input_range
{
private:
  using iterator = meta::detected_t<meta::detected::types::iterator, T>;
  using ResultOfBegin = meta::detected_t<meta::result_of_begin, T&>;
  using value_type = meta::detected_t<meta::detected::types::value_type, T>;

  using lvalue_ref = std::add_lvalue_reference_t<T>;
  using lvalue_const_ref = std::add_lvalue_reference_t<std::add_const_t<T>>;

  using underlying_iterator =
      meta::detected_t<detail::detected::types::underlying_iterator, T>;
  using underlying_sentinel =
      meta::detected_t<detail::detected::types::underlying_sentinel, T>;

  static auto constexpr const is_constructible_from_iterator_sentinel =
      meta::concepts::core::
          is_constructible<T, underlying_iterator, underlying_sentinel>::value;

public:
  using requirements =
      std::tuple<meta::concepts::object::is_semiregular<T>,
                 mgs::concepts::is_iterable<T>>;

  static auto constexpr value =
      is_constructible_from_iterator_sentinel &&
      meta::concepts::object::is_semiregular<T>::value &&
      concepts::is_iterable<T>::value &&
      std::is_same<ResultOfBegin, iterator>::value &&
      meta::concepts::iterator::is_iterator<underlying_iterator>::value &&
      meta::concepts::iterator::is_sentinel<underlying_sentinel,
                                            underlying_iterator>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not a TransformedInputRange");
    static_assert(std::is_same<ResultOfBegin, iterator>::value,
                  "begin must return T::iterator");
    static_assert(is_constructible_from_iterator_sentinel,
                  "T is not Constructible from Iterator/Sentinel pair");
    static_assert(
        meta::concepts::iterator::is_iterator<underlying_iterator>::value,
        "T::underlying_iterator must be an Iterator");
    static_assert(
        meta::concepts::iterator::is_sentinel<underlying_sentinel,
                                              underlying_iterator>::value,
        "T::underlying_sentinel must be a Sentinel<T::underlying_iterator>");
    return 1;
  }
};

}
template <
    typename T,
    std::enable_if_t<concepts::is_transformed_input_range<T>::value, int> = 0>
using TransformedInputRange = T;
}
}
