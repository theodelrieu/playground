#pragma once

#include <type_traits>

#include <mgs/adapters/concepts/transformed_input_adapter.hpp>
#include <mgs/codecs/detail/detected/static_member_functions/create.hpp>
#include <mgs/codecs/output_traits_fwd.hpp>
#include <mgs/meta/detected.hpp>

// template <typename T, InputAdapter I>
// concept CodecOutput = requires (I& adapter) {
//   Same<T, decltype(output_traits<T>::create(adapter))>;
// };

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace concepts
{
template <typename T, typename InputAdapter>
struct is_codec_output
{
  static constexpr auto const value =
      adapters::concepts::is_transformed_input_adapter<InputAdapter>::value &&
      meta::is_detected_exact<T,
                              detail::detected::static_member_functions::create,
                              output_traits<T>,
                              InputAdapter&>::value;
};
}
}
}
}
