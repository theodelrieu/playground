#pragma once

// This concept does not exist in C++20/Ranges, but I believe its place would be
// in core.

#include <tuple>
#include <type_traits>

#include <mgs/meta/void_t.hpp>

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
template <typename T, typename = void>
struct is_complete_type_impl : std::false_type
{
};

template <typename T>
struct is_complete_type_impl<T, void_t<decltype(sizeof(T))>> : std::true_type
{
};
}

template <typename T>
struct is_complete_type : detail::is_complete_type_impl<T>
{
  using requirements = std::tuple<>;

  struct static_assert_t
  {
    static constexpr int trigger()
    {
      static_assert(is_complete_type::value, "T is not a complete type");
      return 1;
    }
  };
};

template <typename T, typename = std::enable_if_t<is_complete_type<T>::value>>
using CompleteType = T;
}
}
}
}
}
