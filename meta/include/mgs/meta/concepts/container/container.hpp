#pragma once

#include <iterator>
#include <type_traits>

#include <mgs/meta/concepts/comparison/equality_comparable.hpp>
#include <mgs/meta/concepts/iterator/forward_iterator.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/free_functions/swap.hpp>
#include <mgs/meta/detected/member_functions/begin.hpp>
#include <mgs/meta/detected/member_functions/cbegin.hpp>
#include <mgs/meta/detected/member_functions/cend.hpp>
#include <mgs/meta/detected/member_functions/empty.hpp>
#include <mgs/meta/detected/member_functions/end.hpp>
#include <mgs/meta/detected/member_functions/max_size.hpp>
#include <mgs/meta/detected/member_functions/size.hpp>
#include <mgs/meta/detected/member_functions/swap.hpp>
#include <mgs/meta/detected/types/const_iterator.hpp>
#include <mgs/meta/detected/types/const_reference.hpp>
#include <mgs/meta/detected/types/difference_type.hpp>
#include <mgs/meta/detected/types/iterator.hpp>
#include <mgs/meta/detected/types/reference.hpp>
#include <mgs/meta/detected/types/size_type.hpp>
#include <mgs/meta/detected/types/value_type.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
namespace container
{
template <typename T, typename = void>
struct is_container : std::false_type
{
};

template <typename T>
struct is_container<
    T,
    std::enable_if_t<
        meta::concepts::iterator::is_forward_iterator<
            meta::detected_t<detected::types::iterator, T>>::value &&
        meta::concepts::iterator::is_forward_iterator<
            meta::detected_t<detected::types::const_iterator, T>>::value>>
{
private:
  using value_type = meta::detected_t<detected::types::value_type, T>;
  using difference_type = meta::detected_t<detected::types::difference_type, T>;
  using reference = meta::detected_t<detected::types::reference, T>;
  using const_reference = meta::detected_t<detected::types::const_reference, T>;
  using iterator = meta::detected_t<detected::types::iterator, T>;
  using const_iterator = meta::detected_t<detected::types::const_iterator, T>;
  using size_type = meta::detected_t<detected::types::size_type, T>;
  using iterator_traits = std::iterator_traits<iterator>;
  using const_iterator_traits = std::iterator_traits<const_iterator>;

  static constexpr auto const type_requirements =
      std::is_same<reference, value_type&>::value &&
      std::is_same<const_reference, value_type const&>::value &&
      std::is_same<value_type, typename iterator_traits::value_type>::value &&
      std::is_same<value_type, typename const_iterator_traits::value_type>::value &&
      std::is_convertible<iterator, const_iterator>::value &&
      std::is_signed<difference_type>::value &&
      std::is_same<difference_type,
                   typename iterator_traits::difference_type>::value &&
      std::is_same<difference_type,
                   typename const_iterator_traits::difference_type>::value;

  static constexpr auto const operation_requirements =
      std::is_default_constructible<T>::value &&
      std::is_copy_constructible<T>::value && std::is_destructible<T>::value &&
      std::is_copy_assignable<T>::value &&
      meta::concepts::comparison::is_equality_comparable<T>::value;

  using begin_t = meta::detected_t<meta::detected::member_functions::begin, T&>;

  static constexpr auto const function_requirements =
      meta::is_detected_exact<iterator,
                              meta::detected::member_functions::begin,
                              T&>::value &&
      meta::is_detected_exact<const_iterator,
                              meta::detected::member_functions::begin,
                              T const&>::value &&
      meta::is_detected_exact<iterator,
                              meta::detected::member_functions::end,
                              T&>::value &&
      meta::is_detected_exact<const_iterator,
                              meta::detected::member_functions::end,
                              T const&>::value &&
      meta::is_detected_exact<const_iterator,
                              meta::detected::member_functions::cbegin,
                              T&>::value &&
      meta::is_detected_exact<const_iterator,
                              meta::detected::member_functions::cend,
                              T&>::value &&
      meta::is_detected_exact<size_type,
                              meta::detected::member_functions::size,
                              T&>::value &&
      meta::is_detected_exact<size_type,
                              meta::detected::member_functions::max_size,
                              T&>::value &&
      meta::is_detected_convertible<bool,
                                    meta::detected::member_functions::empty,
                                    T&>::value &&
      meta::is_detected_exact<void,
                              meta::detected::member_functions::swap,
                              T&,
                              T&>::value &&
      meta::is_detected_exact<void,
                              meta::detected::free_functions::swap,
                              T&,
                              T&>::value;

public:
  static constexpr auto const value =
      type_requirements && operation_requirements && function_requirements;
};
}
}
};
}
}
