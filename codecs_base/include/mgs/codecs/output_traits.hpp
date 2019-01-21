#pragma once

#include <type_traits>

#include <mgs/concepts/transformed_input_range.hpp>
#include <mgs/codecs/detail/default_converter.hpp>
#include <mgs/codecs/detail/detected/static_member_functions/create.hpp>
#include <mgs/codecs/output_traits_fwd.hpp>
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
  template <typename A, typename U = T>
  static auto create(mgs::TransformedInputRange<A>& range)
      -> decltype(detail::default_converter<U>::create(range))
  {
    return detail::default_converter<U>::create(range);
  }
};
}
}
}
