#pragma once

#include <iterator>
#include <type_traits>

#include <b64/detail/distance.hpp>
#include <b64/detail/meta/aliases.hpp>
#include <b64/detail/meta/concepts/input_iterator.hpp>
#include <b64/detail/meta/concepts/random_access_iterator.hpp>
#include <b64/detail/meta/concepts/sentinel.hpp>
#include <b64/detail/meta/concepts/sized_sentinel.hpp>

namespace b64
{
namespace detail
{
template <typename Iterator,
          typename Sentinel,
          std::enable_if_t<is_input_iterator<Iterator>::value &&
                               is_sentinel<Sentinel, Iterator>::value,
                           int> = 0>
void advance_to_impl(Iterator& it, Sentinel const& end, std::input_iterator_tag)
{
  while (it != end)
    ++it;
}

template <typename Iterator,
          typename Sentinel,
          std::enable_if_t<is_random_access_iterator<Iterator>::value &&
                               is_sized_sentinel<Sentinel, Iterator>::value,
                           int> = 0>
void advance_to_impl(Iterator& it,
                     Sentinel const& end,
                     std::random_access_iterator_tag)
{
  it += end - it;
}

template <typename Iterator,
          typename Sentinel,
          std::enable_if_t<is_iterator<Iterator>::value &&
                               is_sentinel<Sentinel, Iterator>::value &&
                               std::is_same<Iterator, Sentinel>::value,
                           int> = 0>
void advance_to(Iterator& it, Sentinel const& sent)
{
  it = sent;
}

template <typename Iterator,
          typename Sentinel,
          std::enable_if_t<is_iterator<Iterator>::value &&
                               is_sentinel<Sentinel, Iterator>::value &&
                               !std::is_same<Iterator, Sentinel>::value,
                           int> = 0>
auto distance(Iterator& it, Sentinel const& end)
    -> decltype(::b64::detail::advance_to_impl(
        it, end, iterator_category_t<std::iterator_traits<Iterator>>{}))
{
  ::b64::detail::advance_to_impl(
      it, end, iterator_category_t<std::iterator_traits<Iterator>>{});
}
}
}
