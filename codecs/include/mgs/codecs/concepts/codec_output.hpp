#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/codecs/detected/static_member_functions/create.hpp>
#include <mgs/codecs/output_traits_fwd.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/ranges/concepts/transformed_input_range.hpp>

// template <typename T, TransformedInputRange I>
// concept CodecOutput = requires (I& range) {
//   Same<T, decltype(codecs::output_traits<T>::create(range))>;
// };

namespace mgs
{
namespace codecs
{
template <typename T, typename TransformedInputRange>
struct is_codec_output
{
private:
  static constexpr auto const has_create_method = meta::is_detected_exact<
      T,
      detected::static_member_functions::create,
      meta::detected_t<codecs::output_traits, T>,
      std::add_lvalue_reference_t<TransformedInputRange>>::value;

public:
  using requirements =
      std::tuple<ranges::is_transformed_input_range<TransformedInputRange>>;

  static constexpr auto const value =
      ranges::is_transformed_input_range<TransformedInputRange>::value &&
      has_create_method;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not a CodecOutput");
    static_assert(
        has_create_method,
        "Invalid or missing static member function: 'T "
        "mgs::codecs::output_traits<T>::create(TransformedInputRange&)'");
    return 1;
  }
};

template <typename T,
          typename TransformedInputRange,
          typename = std::enable_if_t<
              is_codec_output<T, TransformedInputRange>::value>>
using CodecOutput = T;
}
}