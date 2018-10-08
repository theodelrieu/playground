#pragma once

#include <type_traits>

#include <mgs/adapters/detail/detected/types/underlying_iterator.hpp>
#include <mgs/adapters/detail/detected/types/underlying_sentinel.hpp>
#include <mgs/meta/concepts/iterator/iterable.hpp>
#include <mgs/meta/concepts/iterator/iterator.hpp>
#include <mgs/meta/concepts/iterator/random_access_iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/concepts/iterator/sized_sentinel.hpp>
#include <mgs/meta/concepts/object/semiregular.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/function_call.hpp>
#include <mgs/meta/detected/types/value_type.hpp>

// template <typename T>
// concept InputTransformer =
//   using I = typename T::underlying_iterator;
//   using S = typename T::underlying_sentinel;
//   SemiRegular<T> &&
//   Iterable<typename T::value_type> &&
//   SemiRegular<typename T::value_type> &&
//   Iterator<I> &&
//   Sentinel<S, I> &&
//   Constructible<T, I, S> &&
//   RandomAccessIterator<result_of_begin<typename T::value_type>> &&
//   SizedSentinel<
//     result_of_end<typename T::value_type>,
//     result_of_begin<typename T::value_type>> &&
//   requires (T& a, I it, S sent) {
//     Same<typename T::value_type, decltype(a(it, sent))>;
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
struct is_input_transformer : std::false_type
{
};

template <typename T>
struct is_input_transformer<
    T,
    std::enable_if_t<
        meta::is_detected<detail::detected::types::underlying_iterator,
                          T>::value &&
        meta::is_detected<detail::detected::types::underlying_sentinel,
                          T>::value>>
{
  using Iterator =
      meta::detected_t<detail::detected::types::underlying_iterator, T>;
  using Sentinel =
      meta::detected_t<detail::detected::types::underlying_sentinel, T>;

  using value_type = meta::detected_t<meta::detected::types::value_type, T>;
  using value_type_iterator =
      meta::detected_t<meta::result_of_begin, value_type const&>;
  using value_type_sentinel =
      meta::detected_t<meta::result_of_end, value_type const&>;

public:
  static auto constexpr value =
      meta::concepts::iterator::is_iterator<Iterator>::value &&
      meta::concepts::iterator::is_sentinel<Sentinel, Iterator>::value &&
      std::is_constructible<T, Iterator, Sentinel>::value &&
      meta::concepts::object::is_semiregular<T>::value &&
      meta::concepts::iterator::is_iterable<value_type>::value &&
      meta::concepts::object::is_semiregular<value_type>::value &&
      meta::concepts::iterator::is_random_access_iterator<
          value_type_iterator>::value &&
      meta::concepts::iterator::is_sized_sentinel<value_type_sentinel,
                                                  value_type_iterator>::value;
      // meta::is_detected_exact<value_type,
      //                         meta::detected::operators::function_call,
      //                         T&>::value;
};
}
}
}
}
