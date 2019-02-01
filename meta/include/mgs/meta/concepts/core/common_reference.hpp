#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/common_reference.hpp>
#include <mgs/meta/concepts/core/complete_type.hpp>
#include <mgs/meta/concepts/core/convertible_to.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/void_t.hpp>

// https://en.cppreference.com/w/cpp/experimental/ranges/concepts/CommonReference

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
namespace core
{
namespace detail
{
template <typename T, typename U, typename = void>
struct has_common_reference_impl : std::false_type
{
  static constexpr int trigger_static_asserts()
  {
    static_assert(is_detected<common_reference_t, T, U>::value,
                  "T and U do not share a common reference type");
    static_assert(is_detected<common_reference_t, U, T>::value,
                  "U and T do not share a common reference type");
    return 1;
  }
};

template <typename T>
struct S;
template <typename T, typename U>
struct has_common_reference_impl<
    T,
    U,
    void_t<common_reference_t<T, U>, common_reference_t<U, T>>>
{
private:
  using CommonRefTU = common_reference_t<T, U>;
  using CommonRefUT = common_reference_t<U, T>;
  // S<CommonRefTU> s;

public:
  static constexpr auto const value =
      std::is_same<CommonRefTU, CommonRefUT>::value &&
      is_convertible_to<T, CommonRefTU>::value &&
      is_convertible_to<U, CommonRefTU>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(std::is_same<CommonRefTU, CommonRefUT>::value,
                  "common_reference_t<T, U> must be the same then "
                  "common_reference_t<U, T>");
    static_assert(is_convertible_to<T, CommonRefTU>::value,
                  "T must be convertible to common_reference_t<T, U>");
    static_assert(is_convertible_to<U, CommonRefTU>::value,
                  "U must be convertible to common_reference_t<T, U>");
    return 1;
  }
};
}

template <typename T, typename U>
struct has_common_reference : detail::has_common_reference_impl<T, U>
{
  using requirements = std::tuple<>;
};

template <typename T,
          typename U,
          typename = std::enable_if_t<has_common_reference<T, U>::value>>
using CommonReference = T;
}
}
}
}
}
