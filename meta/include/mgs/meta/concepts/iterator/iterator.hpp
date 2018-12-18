#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/core/complete_type.hpp>
#include <mgs/meta/concepts/iterator/iterator_traits.hpp>
#include <mgs/meta/concepts/iterator/weakly_incrementable.hpp>
#include <mgs/meta/concepts/object/semiregular.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/dereference.hpp>
#include <mgs/meta/detected/operators/pre_increment.hpp>

// http://en.cppreference.com/w/cpp/concept/Iterator

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
struct is_iterator
{
private:
  using lvalue_ref = std::add_lvalue_reference_t<T>;

  static constexpr auto const has_dereference =
      is_detected<detected::operators::dereference, lvalue_ref>::value;

public:
  using requirements = std::tuple<is_iterator_traits<std::iterator_traits<T>>,
                                  is_weakly_incrementable<T>>;

  static constexpr auto const value =
      is_iterator_traits<std::iterator_traits<T>>::value &&
      is_weakly_incrementable<T>::value && has_dereference;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not an Iterator");
    static_assert(has_dereference,
                  "Invalid or missing operator: '/* any */ operator*()'");
    return 1;
  }
};

template <typename T, typename = std::enable_if_t<is_iterator<T>::value>>
using Iterator = T;
}
};
}
}
}
