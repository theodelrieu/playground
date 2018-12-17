#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/adapters/concepts/transformed_input_adapter.hpp>
#include <mgs/codecs/detail/detected/static_member_functions/create.hpp>
#include <mgs/codecs/output_traits_fwd.hpp>
#include <mgs/meta/detected.hpp>

// template <typename T, TransformedInputAdapter I>
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
template <typename T, typename TransformedInputAdapter>
struct is_codec_output
{
private:
  static constexpr auto const has_create_method =
      meta::is_detected_exact<T,
                              detail::detected::static_member_functions::create,
                              meta::detected_t<output_traits, T>,
                              TransformedInputAdapter&>::value;

public:
  using requirements =
      std::tuple<adapters::concepts::is_transformed_input_adapter<
          TransformedInputAdapter>>;

  static constexpr auto const value =
      adapters::concepts::is_transformed_input_adapter<
          TransformedInputAdapter>::value &&
      has_create_method;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not a CodecOutput");
    static_assert(has_create_method,
                  "Invalid or missing static member function: 'T "
                  "output_traits<T>::create(TransformedInputAdapter&)'");
    return 1;
  }
};

template <typename T,
          typename TransformedInputAdapter,
          typename = std::enable_if_t<
              is_codec_output<T, TransformedInputAdapter>::value>>
using CodecOutput = T;
}
}
}
}
