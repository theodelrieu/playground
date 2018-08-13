#pragma once

#include <type_traits>

#include <mgs/adapters/detail/detected/member_functions/get.hpp>
#include <mgs/adapters/detail/detected/member_functions/seek_forward.hpp>
#include <mgs/adapters/detail/detected/types/underlying_iterator.hpp>
#include <mgs/adapters/detail/detected/types/underlying_sentinel.hpp>
#include <mgs/meta/concepts/iterator/iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/concepts/object/regular.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/types/difference_type.hpp>
#include <mgs/meta/detected/types/iterator_category.hpp>
#include <mgs/meta/detected/types/reference.hpp>
#include <mgs/meta/detected/types/value_type.hpp>

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
        meta::is_detected<meta::detected::types::value_type, T>::value &&
        meta::is_detected<detail::detected::types::underlying_iterator,
                          T>::value &&
        meta::is_detected<detail::detected::types::underlying_sentinel,
                          T>::value &&
        meta::is_detected<meta::detected::types::difference_type, T>::value>>
{
private:
  using value_type = meta::detected::types::value_type<T>;
  using difference_type = meta::detected::types::difference_type<T>;

public:
  static auto constexpr value =
      std::is_constructible<
          T,
          detail::detected::types::underlying_iterator<T>,
          detail::detected::types::underlying_sentinel<T>>::value &&
      meta::is_detected<detail::detected::member_functions::get,
                        T const&>::value &&
      meta::is_detected_exact<void,
                              detail::detected::member_functions::seek_forward,
                              T&,
                              difference_type>::value;
};
}
}
}
}
