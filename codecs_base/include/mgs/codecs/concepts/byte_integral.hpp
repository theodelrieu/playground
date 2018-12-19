#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/core/complete_type.hpp>
#include <mgs/meta/void_t.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace concepts
{
namespace detail
{
template <typename, typename = void>
struct is_byte_integral_impl : std::false_type
{
  static constexpr int trigger_static_asserts()
  {
    return 1;
  }
};

template <typename T>
struct is_byte_integral_impl<
    T,
    meta::void_t<meta::concepts::core::CompleteType<T>>>
{
  private:
    static constexpr auto const is_integral = std::is_integral<T>::value;
    static constexpr auto const is_not_bool = !std::is_same<T, bool>::value;

  public:
    static constexpr auto const value =
        is_integral && is_not_bool && sizeof(T) == 1;

    static constexpr int trigger_static_asserts()
    {
      static_assert(is_integral, "T must be an Integral");
      static_assert(is_not_bool, "T cannot be 'bool'");
      static_assert(sizeof(T) == 1, "T must have a size of 1");
      return 1;
    }
};
}

template <typename T>
struct is_byte_integral: detail::is_byte_integral_impl<T>
{
  using requirements = std::tuple<>;

  static constexpr int trigger_static_asserts()
  {
    return detail::is_byte_integral_impl<T>::trigger_static_asserts();
  }
};

template <typename T, typename = std::enable_if_t<is_byte_integral<T>::value>>
using ByteIntegral = T;
}
}
}
}
