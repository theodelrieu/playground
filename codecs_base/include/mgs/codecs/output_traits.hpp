#pragma once

#include <type_traits>

#include <mgs/adapters/concepts/input_adapter.hpp>
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
  template <typename InputAdapter,
            typename U = T,
            typename = std::enable_if_t<
                adapters::concepts::is_input_adapter<InputAdapter>::value>>
  static auto create(InputAdapter& adapter)
      -> decltype(detail::default_converter<U>::create(adapter))
  {
    return detail::default_converter<U>::create(adapter);
  }
};
}
}
}
