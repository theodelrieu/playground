#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/adapters/concepts/transformed_input_adapter.hpp>
#include <mgs/meta/call_std/begin.hpp>
#include <mgs/meta/concepts/iterator/iterable.hpp>
#include <mgs/meta/detected/types/iterator.hpp>

// template <typename T>
// concept IterableTransformedInputAdapter =
//   TransformedInputAdapter<T> &&
//   Iterable<T> &&
//   Same<typename T::iterator, result_of_begin<T>>;

namespace mgs
{
inline namespace v1
{
namespace adapters
{
namespace concepts
{
template <typename T>
struct is_iterable_transformed_input_adapter
{
private:
  using I = meta::detected_t<meta::detected::types::iterator, T>;
  using ResultOfBegin = meta::detected_t<meta::result_of_begin, T&>;

public:
  using requirements = std::tuple<is_transformed_input_adapter<T>,
                                  meta::concepts::iterator::is_iterable<T>>;

  static constexpr bool const value =
      is_transformed_input_adapter<T>::value &&
      meta::concepts::iterator::is_iterable<T>::value &&
      std::is_same<ResultOfBegin, I>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not an IterableTransformedInputAdapter");
    static_assert(std::is_same<ResultOfBegin, I>::value,
                  "begin must return T::iterator");
    return 1;
  }
};

template <typename T,
          typename =
              std::enable_if_t<is_iterable_transformed_input_adapter<T>::value>>
using IterableTransformedInputAdapter = T;
}
}
}
}
