#pragma once

#include <type_traits>

#include <mgs/codecs/detail/default_converter.hpp>
#include <mgs/codecs/output_traits_fwd.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/ranges/concepts/transformed_input_range.hpp>

namespace mgs
{
namespace codecs
{
template <typename T, typename>
struct output_traits
{
  template <typename A, typename U = T>
  static auto create(mgs::ranges::TransformedInputRange<A>& range)
      -> decltype(detail::default_converter<U>::create(range))
  {
    return detail::default_converter<U>::create(range);
  }
};
}
}