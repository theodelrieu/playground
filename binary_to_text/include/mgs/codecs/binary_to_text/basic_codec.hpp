#pragma once

#include <mgs/codecs/basic_codec.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace binary_to_text
{
template <typename CodecTraits>
class basic_codec : public codecs::basic_codec<CodecTraits>
{
  using base = codecs::basic_codec<CodecTraits>;

public:
  template <typename T = typename base::default_encoded_output,
            std::size_t N = 0>
  static auto encode(char const (&tab)[N])
      -> decltype(base::template encode<T>(std::declval<char const*>(),
                                           std::declval<char const*>()))
  {
    auto const end_it = std::find(std::begin(tab), std::end(tab), '\0');
    return base::template encode<T>(std::begin(tab), end_it);
  }

  template <typename T = typename base::default_decoded_output,
            std::size_t N = 0>
  static auto decode(char const (&tab)[N])
      -> decltype(base::template decode<T>(std::declval<char const*>(),
                                           std::declval<char const*>()))
  {
    auto const end_it = std::find(std::begin(tab), std::end(tab), '\0');
    return base::template decode<T>(std::begin(tab), end_it);
  }
};
}
}
}
}
