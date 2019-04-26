#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/common_reference.hpp>
#include <mgs/meta/concepts/complete_type.hpp>
#include <mgs/meta/concepts/convertible_to.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/void_t.hpp>

// https://en.cppreference.com/w/cpp/experimental/ranges/concepts/CommonReference

namespace mgs
{
namespace detail
{
template <typename T, typename U, typename = void>
struct has_common_reference_impl : std::false_type
{
  static constexpr int trigger_static_asserts()
  {
    static_assert(meta::is_detected<meta::common_reference_t, T, U>::value,
                  "T and U do not share a common reference type");
    static_assert(meta::is_detected<meta::common_reference_t, U, T>::value,
                  "U and T do not share a common reference type");
    return 1;
  }
};

template <typename T, typename U>
struct has_common_reference_impl<
    T,
    U,
    meta::void_t<meta::common_reference_t<T, U>, meta::common_reference_t<U, T>>>
{
private:
  using CommonRefTU = meta::common_reference_t<T, U>;
  using CommonRefUT = meta::common_reference_t<U, T>;

public:
  static constexpr auto const value =
      std::is_same<CommonRefTU, CommonRefUT>::value &&
      meta::concepts::is_convertible_to<T, CommonRefTU>::value &&
      meta::concepts::is_convertible_to<U, CommonRefTU>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(std::is_same<CommonRefTU, CommonRefUT>::value,
                  "common_reference_t<T, U> must be the same then "
                  "common_reference_t<U, T>");
    static_assert(meta::concepts::is_convertible_to<T, CommonRefTU>::value,
                  "T must be convertible to common_reference_t<T, U>");
    static_assert(meta::concepts::is_convertible_to<U, CommonRefTU>::value,
                  "U must be convertible to common_reference_t<T, U>");
    return 1;
  }
};
}

namespace meta
{
namespace concepts
{
template <typename T, typename U>
struct has_common_reference : detail::has_common_reference_impl<T, U>
{
  using requirements = std::tuple<>;
};

template <typename T, typename U>
constexpr auto has_common_reference_v = has_common_reference<T, U>::value;
}

template <typename T,
          typename U,
          typename =
              std::enable_if_t<concepts::has_common_reference<T, U>::value>>
using CommonReference = T;
}
}
