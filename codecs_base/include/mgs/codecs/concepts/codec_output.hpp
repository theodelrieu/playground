#pragma once

#include <type_traits>

#include <mgs/adapters/concepts/iterable_input_adapter.hpp>
#include <mgs/codecs/detail/detected/static_member_functions/create.hpp>
#include <mgs/codecs/detail/detected/types/output_traits.hpp>
#include <mgs/meta/call_std/begin.hpp>
#include <mgs/meta/call_std/end.hpp>
#include <mgs/meta/detected.hpp>

// template <typename T, InputAdapter IA>
// concept CodecOutput = requires (IA const& adapter) {
//   Same<T, decltype(codec_output<T>::create(begin(adapter), end(adapter)))>;
// };

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace concepts
{
template <typename T, typename IA, typename = void>
struct is_codec_output : std::false_type
{
};

template <typename T, typename IA>
struct is_codec_output<
    T,
    IA,
    std::enable_if_t<adapters::concepts::is_iterable_input_adapter<IA>::value>>
{
private:
  using output_traits_t =
      meta::detected_t<detail::detected::types::output_traits, T>;

  using output_t =
      meta::detected_t<detail::detected::static_member_functions::create,
                       output_traits_t,
                       meta::result_of_begin_t<IA const&>,
                       meta::result_of_end_t<IA const&>>;

public:
  static constexpr auto const value = std::is_same<output_t, T>::value;
};
}
}
}
}
