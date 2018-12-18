#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/core/complete_type.hpp>
#include <mgs/meta/concepts/object/semiregular.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/post_increment.hpp>
#include <mgs/meta/detected/operators/pre_increment.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
namespace iterator
{
template <typename T>
struct is_weakly_incrementable
{
private:
  using lvalue_ref = std::add_lvalue_reference_t<T>;

  static constexpr auto const has_pre_increment =
      is_detected_exact<lvalue_ref,
                        detected::operators::pre_increment,
                        lvalue_ref>::value;

  static constexpr auto const has_post_increment =
      is_detected<detected::operators::post_increment, lvalue_ref>::value;

public:
  using requirements = std::tuple<object::is_semiregular<T>>;

  static constexpr auto const value = object::is_semiregular<T>::value &&
                                      has_pre_increment && has_post_increment;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not WeaklyIncrementable");
    static_assert(has_pre_increment,
                  "Invalid or missing operator: 'T& operator++()'");
    static_assert(has_post_increment,
                  "Invalid or missing operator: '/* any */ operator++(int)'");
    return 1;
  }
};

template <typename T,
          typename = std::enable_if_t<is_weakly_incrementable<T>::value>>
using WeaklyIncrementable = T;
}
};
}
}
}
