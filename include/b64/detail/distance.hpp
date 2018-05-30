#pragma once

#include <type_traits>

#include <b64/detail/meta/aliases.hpp>

#include <b64/detail/meta/concepts/iterator.hpp>
#include <b64/detail/meta/concepts/random_access_iterator.hpp>
#include <b64/detail/meta/concepts/sentinel.hpp>
#include <b64/detail/meta/concepts/sized_sentinel.hpp>

namespace b64
{
namespace detail
{
template <typename Iterator,
          typename Sentinel,
          std::enable_if_t<is_random_access_iterator<Iterator>::value &&
                               is_sized_sentinel<Sentinel, Iterator>::value,
                           int> = 0>
auto distance_impl(Iterator const& it,
                   Sentinel const& end,
                   std::random_access_iterator_tag)
    -> difference_type_t<std::iterator_traits<Iterator>>
{
  return end - it;
}

template <typename Iterator,
          typename Sentinel,
          std::enable_if_t<is_input_iterator<Iterator>::value &&
                               is_sentinel<Sentinel, Iterator>::value,
                           int> = 0>
auto distance_impl(Iterator const& it,
                   Sentinel const& end,
                   std::input_iterator_tag)
    -> difference_type_t<std::iterator_traits<Iterator>>
{
  auto tmp = it;
  auto n = difference_type_t<std::iterator_traits<Iterator>>{0};

  while (tmp != end)
  {
    ++tmp;
    ++n;
  }
  return n;
}

template <typename Iterator,
          typename Sentinel,
          std::enable_if_t<is_iterator<Iterator>::value &&
                           is_sentinel<Sentinel, Iterator>::value &&
                           std::is_same<Iterator, Sentinel>::value, int> = 0>
auto distance(Iterator const& it, Sentinel const& sent)
    -> decltype(std::distance(it, sent))
{
  return std::distance(it, sent);
}

template <typename Iterator,
          typename Sentinel,
          std::enable_if_t<is_iterator<Iterator>::value &&
                           is_sentinel<Sentinel, Iterator>::value &&
                           !std::is_same<Iterator, Sentinel>::value, int> = 0>
auto distance(Iterator const& it, Sentinel const& end)
    -> decltype(::b64::detail::distance_impl(it, end, iterator_category_t<std::iterator_traits<Iterator>>{}))
{
}
}
}
