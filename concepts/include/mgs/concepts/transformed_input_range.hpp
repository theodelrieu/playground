#pragma once

#include <cstddef>
#include <tuple>
#include <type_traits>

#include <mgs/concepts/detail/detected/types/underlying_iterator.hpp>
#include <mgs/concepts/detail/detected/types/underlying_sentinel.hpp>
#include <mgs/meta/concepts/range.hpp>
#include <mgs/meta/call_std/begin.hpp>
#include <mgs/meta/concepts/constructible.hpp>
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/sentinel.hpp>
#include <mgs/meta/concepts/semiregular.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/types/iterator.hpp>

// clang-format off
//
// template <typename T>
// concept TransformedInputRange =
//   std::Semiregular<T> &&
//   Range<T> &&
//   requires {
//      std::InputIterator<typename T::underlying_iterator>;
//      std::Sentinel<typename T::underlying_sentinel, typename T::underlying_iterator>;
//      std::Same<typename T::iterator, result_of_begin<T>>;
//      std::Constructible<T, typename T::underlying_iterator, typename T::underlying_sentinel>;
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

  using lvalue_ref = std::add_lvalue_reference_t<T>;

  using ResultOfBegin = meta::detected_t<meta::result_of_begin, lvalue_ref>;

  using underlying_iterator =
      meta::detected_t<detail::detected::types::underlying_iterator, T>;
  using underlying_sentinel =
      meta::detected_t<detail::detected::types::underlying_sentinel, T>;

  static auto constexpr const is_constructible_from_iterator_sentinel =
      meta::concepts::
          is_constructible<T, underlying_iterator, underlying_sentinel>::value;

public:
  using requirements = std::tuple<meta::concepts::is_semiregular<T>,
                                  meta::concepts::is_range<T>>;

  static auto constexpr value =
      is_constructible_from_iterator_sentinel &&
      meta::concepts::is_semiregular<T>::value &&
      meta::concepts::is_range<T>::value &&
      std::is_same<ResultOfBegin, iterator>::value &&
      meta::concepts::is_input_iterator<underlying_iterator>::value &&
      meta::concepts::is_sentinel<underlying_sentinel,
                                            underlying_iterator>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not a TransformedInputRange");
    static_assert(std::is_same<ResultOfBegin, iterator>::value,
                  "begin must return T::iterator");
    static_assert(is_constructible_from_iterator_sentinel,
                  "T is not Constructible from Iterator/Sentinel pair");
    static_assert(
        meta::concepts::is_input_iterator<underlying_iterator>::value,
        "T::underlying_iterator must be an InputIterator");
    static_assert(
        meta::concepts::is_sentinel<underlying_sentinel,
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
