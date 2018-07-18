#pragma once

#include <type_traits>

#include <mgs/detail/meta/aliases.hpp>
#include <mgs/detail/meta/concepts/iterable.hpp>
#include <mgs/detail/meta/concepts/iterator.hpp>
#include <mgs/detail/meta/concepts/random_access_iterator.hpp>
#include <mgs/detail/meta/concepts/semiregular.hpp>
#include <mgs/detail/meta/concepts/sentinel.hpp>
#include <mgs/detail/meta/concepts/sized_sentinel.hpp>
#include <mgs/detail/meta/detected.hpp>

// template <typename T, Iterator I, Sentinel<I> S>
// concept BlockInputTransformer = requires(T const& a) {
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
namespace detail
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
    std::enable_if_t<is_iterator<Iterator>::value &&
                     is_sentinel<Sentinel, Iterator>::value &&
                     is_semiregular<T>::value &&
                     is_iterable<detected_t<value_type_t, T>>::value &&
                     is_semiregular<detected_t<value_type_t, T>>::value>>
{
  using value_type = typename T::value_type;
  using value_type_iterator = detail2::result_of_begin_t<value_type const>;
  using value_type_sentinel = detail2::result_of_end_t<value_type const>;

public:
  static auto constexpr value =
      is_random_access_iterator<value_type_iterator>::value &&
      is_sized_sentinel<value_type_sentinel, value_type_iterator>::value &&
      is_detected_exact<value_type,
                        call_operator_t,
                        T const&,
                        Iterator&,
                        Sentinel>::value;
};
}
}
}
