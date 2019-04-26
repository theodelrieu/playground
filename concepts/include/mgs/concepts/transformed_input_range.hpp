#pragma once

#include <cstddef>
#include <tuple>
#include <type_traits>

#include <mgs/concepts/detected/types/underlying_iterator.hpp>
#include <mgs/concepts/detected/types/underlying_sentinel.hpp>
#include <mgs/meta/concepts/constructible.hpp>
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/range.hpp>
#include <mgs/meta/concepts/semiregular.hpp>
#include <mgs/meta/concepts/sentinel.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/types/iterator.hpp>
#include <mgs/meta/iterator_t.hpp>

// clang-format off
//
// template <typename T>
// concept TransformedInputRange =
//   std::Semiregular<T> &&
//   std::Range<T> &&
//   requires {
//      std::InputIterator<typename T::underlying_iterator>;
//      std::Sentinel<typename T::underlying_sentinel, typename T::underlying_iterator>;
//      std::Constructible<T, typename T::underlying_iterator, typename T::underlying_sentinel>;
//   };
//
// clang-format on

namespace mgs
{
template <typename T>
struct is_transformed_input_range
{
private:
  using iterator = meta::detected_t<meta::iterator_t, T>;

  using underlying_iterator =
      meta::detected_t<detected::types::underlying_iterator, T>;
  using underlying_sentinel =
      meta::detected_t<detected::types::underlying_sentinel, T>;

  static auto constexpr const is_constructible_from_iterator_sentinel =
      meta::
          is_constructible<T, underlying_iterator, underlying_sentinel>::value;

public:
  using requirements = std::tuple<meta::is_semiregular<T>,
                                  meta::is_range<T>>;

  static auto constexpr value =
      is_constructible_from_iterator_sentinel &&
      meta::is_semiregular<T>::value &&
      meta::is_range<T>::value &&
      meta::is_input_iterator<underlying_iterator>::value &&
      meta::is_sentinel<underlying_sentinel,
                                  underlying_iterator>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not a TransformedInputRange");
    static_assert(is_constructible_from_iterator_sentinel,
                  "T is not Constructible from Iterator/Sentinel pair");
    static_assert(
        meta::is_input_iterator<underlying_iterator>::value,
        "T::underlying_iterator must be an InputIterator");
    static_assert(
        meta::is_sentinel<underlying_sentinel,
                                              underlying_iterator>::value,
        "T::underlying_sentinel must be a Sentinel<T::underlying_iterator>");
    return 1;
  }
};

template <typename T>
constexpr auto is_transformed_input_range_v =
    is_transformed_input_range<T>::value;

template <
    typename T,
    std::enable_if_t<is_transformed_input_range<T>::value, int> = 0>
using TransformedInputRange = T;
}
