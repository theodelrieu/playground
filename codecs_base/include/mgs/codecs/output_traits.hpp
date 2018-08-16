#pragma once

#include <type_traits>
#include <utility>

#include <mgs/codecs/detail/default_converter.hpp>
#include <mgs/codecs/output_traits_fwd.hpp>
#include <mgs/meta/concepts/iterator/iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
template <typename T, typename>
struct output_traits
{
  // TODO sfinae-correctness
  template <typename Iterator,
            typename = std::enable_if_t<
                meta::concepts::iterator::is_iterator<Iterator>::value>>
  static auto create(Iterator it, Iterator end)
      -> decltype(detail::default_converter<T>::create(std::move(it),
                                                       std::move(end)))
  {
    return detail::default_converter<T>::create(std::move(it), std::move(end));
  }
};
}
}
}
