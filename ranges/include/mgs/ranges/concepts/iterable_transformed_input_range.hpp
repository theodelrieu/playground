#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/ranges/concepts/transformed_input_range.hpp>
#include <mgs/meta/call_std/begin.hpp>
#include <mgs/meta/concepts/iterator/iterable.hpp>
#include <mgs/meta/detected/types/iterator.hpp>
#include <mgs/meta/detected/types/value_type.hpp>

// template <typename T, OutputIterator OI = typename T::value_type*>
// concept IterableTransformedInputRange =
//   TransformedInputRange<T> &&
//   Iterable<T> &&
//   Same<typename T::iterator, result_of_begin<T>>;

namespace mgs
{
inline namespace v1
{
namespace ranges
{
namespace concepts
{
template <typename T,
          typename OutputIterator = std::add_pointer_t<
              meta::detected_t<meta::detected::types::value_type, T>>>
struct is_iterable_transformed_input_range
{
private:
  using I = meta::detected_t<meta::detected::types::iterator, T>;
  using ResultOfBegin = meta::detected_t<meta::result_of_begin, T&>;

public:
  using requirements = std::tuple<is_transformed_input_range<T, OutputIterator>,
                                  meta::concepts::iterator::is_iterable<T>>;

  static constexpr bool const value =
      is_transformed_input_range<T, OutputIterator>::value &&
      meta::concepts::iterator::is_iterable<T>::value &&
      std::is_same<ResultOfBegin, I>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not an IterableTransformedInputRange");
    static_assert(std::is_same<ResultOfBegin, I>::value,
                  "begin must return T::iterator");
    return 1;
  }
};

template <typename T,
          typename OutputIterator = std::add_pointer_t<
              meta::detected_t<meta::detected::types::value_type, T>>,
          typename = std::enable_if_t<
              is_iterable_transformed_input_range<T, OutputIterator>::value>>
using IterableTransformedInputRange = T;
}
}
}
}