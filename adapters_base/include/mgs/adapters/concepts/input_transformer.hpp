#pragma once

#include <type_traits>

#include <mgs/meta/aliases/operators/function_call.hpp>
#include <mgs/meta/aliases/types/value_type.hpp>
#include <mgs/meta/concepts/iterator/iterable.hpp>
#include <mgs/meta/concepts/iterator/iterator.hpp>
#include <mgs/meta/concepts/iterator/random_access_iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/concepts/iterator/sized_sentinel.hpp>
#include <mgs/meta/concepts/object/semiregular.hpp>
#include <mgs/meta/detected.hpp>

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
        meta::iterator_concepts::is_iterator<Iterator>::value &&
        meta::iterator_concepts::is_sentinel<Sentinel, Iterator>::value &&
        meta::object_concepts::is_semiregular<T>::value &&
        meta::iterator_concepts::is_iterable<
            meta::detected_t<meta::type_aliases::value_type, T>>::value &&
        meta::object_concepts::is_semiregular<
            meta::detected_t<meta::type_aliases::value_type, T>>::value>>
{
  using value_type = meta::type_aliases::value_type<T>;
  using value_type_iterator = meta::result_of_begin_t<value_type const>;
  using value_type_sentinel = meta::result_of_end_t<value_type const>;

public:
  static auto constexpr value =
      meta::iterator_concepts::is_random_access_iterator<
          value_type_iterator>::value &&
      meta::iterator_concepts::is_sized_sentinel<value_type_sentinel,
                                                 value_type_iterator>::value &&
      meta::is_detected_exact<value_type,
                              meta::operator_aliases::function_call,
                              T const&,
                              Iterator&,
                              Sentinel>::value;
};
}
}
}
}
