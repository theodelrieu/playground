#pragma once

#include <type_traits>

#include <mgs/adapters/aliases/member_functions/get.hpp>
#include <mgs/adapters/aliases/member_functions/seek_forward.hpp>
#include <mgs/adapters/aliases/types/underlying_iterator.hpp>
#include <mgs/adapters/aliases/types/underlying_sentinel.hpp>
#include <mgs/meta/aliases/types/difference_type.hpp>
#include <mgs/meta/aliases/types/iterator_category.hpp>
#include <mgs/meta/aliases/types/reference.hpp>
#include <mgs/meta/aliases/types/value_type.hpp>
#include <mgs/meta/concepts/iterator/iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/concepts/object/regular.hpp>
#include <mgs/meta/detected.hpp>

// template <typename T>
// concept InputAdapter = requires(T const& v, T& u) {
//  requires Regular<T>;
//  typename T::value_type;
//  typename T::difference_type;
//  typename T::underlying_iterator;
//  typename T::underlying_sentinel;
//  requires Constructible<T, T::underlying_iterator, T::underlying_sentinel>;
//  requires(typename T::difference_type n) {
//     v.get();
//     u.seek_forward(n);
//     requires { v.get() } -> value_type const&;
//     requires { u.seek_forward(n) } -> void;
//   }
// }

namespace mgs
{
inline namespace v1
{
namespace adapters
{
namespace concepts
{
template <typename T, typename = void>
struct is_input_adapter : std::false_type
{
};

template <typename T>
struct is_input_adapter<
    T,
    std::enable_if_t<
        meta::object_concepts::is_regular<T>::value &&
        meta::is_detected<meta::type_aliases::value_type, T>::value &&
        meta::is_detected<type_aliases::underlying_iterator, T>::value &&
        meta::is_detected<type_aliases::underlying_sentinel, T>::value &&
        meta::is_detected<meta::type_aliases::difference_type, T>::value>>
{
private:
  using value_type = meta::type_aliases::value_type<T>;
  using difference_type = meta::type_aliases::difference_type<T>;

public:
  static auto constexpr value =
      std::is_constructible<T,
                            type_aliases::underlying_iterator<T>,
                            type_aliases::underlying_sentinel<T>>::value &&
      meta::is_detected<member_functions_aliases::get, T const&>::value &&
      meta::is_detected_exact<void,
                              member_functions_aliases::seek_forward,
                              T&,
                              difference_type>::value;
};
}
}
}
}
