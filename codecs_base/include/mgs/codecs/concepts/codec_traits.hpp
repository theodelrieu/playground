#pragma once

#include <type_traits>

#include <mgs/codecs/detail/detected/static_member_functions/create.hpp>
#include <mgs/codecs/output_traits_fwd.hpp>
#include <mgs/meta/concepts/iterator/input_iterator.hpp>
#include <mgs/meta/detected.hpp>

// template <typename T, Iterator I>
// concept CodecTraits = requires (I begin, I end) {
//   Same<T, decltype(output_traits<T>::create(begin, end))>;
// };

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace concepts
{
template <typename T, typename InputIterator>
struct is_codec_output
{
  static constexpr auto const value =
      meta::concepts::iterator::is_input_iterator<InputIterator>::value &&
      meta::is_detected_exact<T,
                              detail::detected::static_member_functions::create,
                              output_traits<T>,
                              InputIterator,
                              InputIterator>::value;
};
}
}
}
}
