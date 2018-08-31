#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/comparison/equality_comparable.hpp>
#include <mgs/meta/concepts/object/semiregular.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
namespace object
{
template <typename T, typename = void>
struct is_regular
{
  static constexpr auto const value =
      is_semiregular<T>::value && comparison::is_equality_comparable<T>::value;

  using requirements =
      std::tuple<is_semiregular<T>, comparison::is_equality_comparable<T>>;
};

template <typename T, typename = std::enable_if_t<is_regular<T>::value>>
using Regular = T;
}
};
}
}
}
