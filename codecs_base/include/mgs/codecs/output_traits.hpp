#pragma once

#include <type_traits>

#include <mgs/adapters/concepts/transformed_input_adapter.hpp>
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
  static auto create(adapters::concepts::TransformedInputAdapter<A>& adapter)
      -> decltype(detail::default_converter<U>::create(adapter))
  {
    return detail::default_converter<U>::create(adapter);
  }
};
}
}
}
