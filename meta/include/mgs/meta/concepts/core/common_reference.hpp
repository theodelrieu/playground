#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/common_reference.hpp>
#include <mgs/meta/concepts/core/complete_type.hpp>
#include <mgs/meta/concepts/core/convertible_to.hpp>
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
};

template <typename T, typename U>
struct has_common_reference_impl<
    T,
    U,
    void_t<meta::common_reference_t<T, U>, meta::common_reference_t<U, T>>>
{
  // TODO convertible_to
  static constexpr auto const value =
      std::is_same<meta::common_reference_t<T, U>,
                   meta::common_reference_t<U, T>>::value &&
      is_convertible_to<T, meta::common_reference_t<T, U>>::value &&
      i<U, meta::common_reference_t<T, U>>::value;
};
}
}
}
}
}
}
