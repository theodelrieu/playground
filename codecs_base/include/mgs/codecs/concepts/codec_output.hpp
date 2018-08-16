#pragma once

#include <type_traits>

#include <mgs/codecs/detail/detected/static_member_functions/create.hpp>
#include <mgs/codecs/output_traits_fwd.hpp>
#include <mgs/meta/concepts/iterator/input_iterator.hpp>
#include <mgs/meta/detected.hpp>

// template <typename T, Iterator I>
// concept CodecOutput = requires (I begin, I end) {
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
template <typename T, typename InputIterator, typename = void>
struct is_codec_output : std::false_type
{
};

template <typename T, typename InputIterator>
struct is_codec_output<
    T,
    InputIterator,
    std::enable_if_t<
        meta::concepts::iterator::is_input_iterator<InputIterator>::value>>
{
  static constexpr auto const value = std::is_same<
      meta::detected_t<detail::detected::static_member_functions::create,
                       output_traits<T>,
                       InputIterator,
                       InputIterator>,
      T>::value;
};
}
}
}
}
