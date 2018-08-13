#pragma once

#include <mgs/codecs/output_traits_fwd.hpp>
#include <mgs/meta/concepts/iterator/iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
// TODO namespace detail, support types by default (Containers + std::array
// special case)
template <typename T>
struct output_traits<T, std::enable_if_t<sizeof(T) == 0>>
{
  template <typename Iterator, typename Sentinel>
  static T create(Iterator it, Sentinel end)
  {
    static_assert(meta::concepts::iterator::is_iterator<Iterator>::value,
                  "Iterator is not an iterator");
    static_assert(meta::concepts::iterator::is_sentinel<Sentinel, Iterator>::value,
                  "Sentinel is not an sentinel");
    return {};
  }
};
}
}
}
