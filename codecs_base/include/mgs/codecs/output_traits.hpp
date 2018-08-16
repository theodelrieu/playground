#pragma once

#include <type_traits>
#include <utility>

#include <mgs/codecs/detail/default_converter.hpp>
#include <mgs/codecs/detail/detected/static_member_functions/create.hpp>
#include <mgs/codecs/output_traits_fwd.hpp>
#include <mgs/meta/concepts/iterator/iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/detected.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
template <typename T, typename>
struct output_traits
{
  template <typename Iterator,
            typename U = T,
            typename = std::enable_if_t<
                meta::concepts::iterator::is_iterator<Iterator>::value>>
  static auto create(Iterator it, Iterator end)
      -> decltype(detail::default_converter<U>::create(std::move(it),
                                                       std::move(end)))
  {
    return detail::default_converter<U>::create(std::move(it), std::move(end));
  }
};
}
}
}
