#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/constructible.hpp>
#include <mgs/meta/concepts/convertible_to.hpp>
#include <mgs/meta/concepts/move_constructible.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
template <typename T>
struct is_copy_constructible
{
private:
  using lvalue_ref = std::add_lvalue_reference_t<T>;
  using const_lvalue_ref = std::add_lvalue_reference_t<std::add_const_t<T>>;

public:
  using requirements = std::tuple<is_move_constructible<T>,
                                  is_constructible<T, lvalue_ref>,
                                  is_convertible_to<lvalue_ref, T>,
                                  is_constructible<T, const_lvalue_ref>,
                                  is_convertible_to<const_lvalue_ref, T>,
                                  is_constructible<T, std::add_const_t<T>>,
                                  is_convertible_to<std::add_const_t<T>, T>>;

  static constexpr auto const value =
      is_move_constructible<T>::value &&
      is_constructible<T, lvalue_ref>::value &&
      is_convertible_to<lvalue_ref, T>::value &&
      is_constructible<T, const_lvalue_ref>::value &&
      is_convertible_to<const_lvalue_ref, T>::value &&
      is_constructible<T, std::add_const_t<T>>::value &&
      is_convertible_to<std::add_const_t<T>, T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not CopyConstructible");
    return 1;
  }
};
}

template <typename T,
          typename =
              std::enable_if_t<concepts::is_copy_constructible<T>::value>>
using CopyConstructible = T;
}
}
}