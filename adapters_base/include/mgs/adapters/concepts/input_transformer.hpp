#pragma once

#include <type_traits>

#include <mgs/meta/concepts/iterator/iterable.hpp>
#include <mgs/meta/concepts/iterator/iterator.hpp>
#include <mgs/meta/concepts/iterator/random_access_iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/concepts/iterator/sized_sentinel.hpp>
#include <mgs/meta/concepts/object/semiregular.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/function_call.hpp>
#include <mgs/meta/detected/types/value_type.hpp>

// template <typename T, Iterator I, Sentinel<I> S>
// concept InputTransformer = requires(T const& a) {
//   requires SemiRegular<T>;
//   typename T::value_type;
//   requires Iterable<T::value_type>;
//   requires SemiRegular<T::value_type>;
//
//   requires (T::value_type v) {
//     { begin(v) } -> RandomAccessIterator;
//     { end(v) } -> SizedSentinel<decltype(begin(v))>;
//   }
//
//   requires (I& it, S sent) {
//     a.process(it, sent);
//     { a.process(it, sent) } -> T::value_type;
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
template <typename T, typename Iterator, typename Sentinel, typename = void>
struct is_input_transformer : std::false_type
{
};

template <typename T, typename Iterator, typename Sentinel>
struct is_input_transformer<
    T,
    Iterator,
    Sentinel,
    std::enable_if_t<
        meta::concepts::iterator::is_iterator<Iterator>::value &&
        meta::concepts::iterator::is_sentinel<Sentinel, Iterator>::value &&
        meta::concepts::object::is_semiregular<T>::value &&
        meta::concepts::iterator::is_iterable<
            meta::detected_t<meta::detected::types::value_type, T>>::value &&
        meta::concepts::object::is_semiregular<
            meta::detected_t<meta::detected::types::value_type, T>>::value>>
{
  using value_type = meta::detected::types::value_type<T>;
  using value_type_iterator = meta::result_of_begin_t<value_type const>;
  using value_type_sentinel = meta::result_of_end_t<value_type const>;

public:
  static auto constexpr value =
      meta::concepts::iterator::is_random_access_iterator<
          value_type_iterator>::value &&
      meta::concepts::iterator::is_sized_sentinel<value_type_sentinel,
                                                  value_type_iterator>::value &&
      meta::is_detected_exact<value_type,
                              meta::detected::operators::function_call,
                              T const&,
                              Iterator&,
                              Sentinel>::value;
};
}
}
}
}
